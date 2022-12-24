#include <utility>
#include "./execution_client.h"

namespace ara
{
    namespace exec
    {
        const ExecErrorDomain ExecutionClient::cErrorDomain;

        ExecutionClient::ExecutionClient(
            core::InstanceSpecifier instanceSpecifier,
            com::someip::rpc::RpcClient *rpcClient,
            int64_t timeout) : mInstanceSpecifier{instanceSpecifier},
                               mRpcClient{rpcClient},
                               mTimeout{timeout > 0 ? timeout : throw std::invalid_argument("Invalid timeout.")}
        {
            auto _handler{
                std::bind(
                    &ExecutionClient::reportExecutionStateHandler,
                    this, std::placeholders::_1)};
            mRpcClient->SetHandler(cServiceId, cMethodId, _handler);
        }

        ExecException ExecutionClient::generateException(
            ExecErrc executionErrorCode) const
        {
            auto _code{static_cast<core::ErrorDomain::CodeType>(executionErrorCode)};
            core::ErrorCode _errorCode(_code, cErrorDomain);
            ExecException _result(_errorCode);

            return _result;
        }

        void ExecutionClient::reportExecutionStateHandler(
            const com::someip::rpc::SomeIpRpcMessage &message)
        {
            const std::vector<u_int8_t> &cRpcPayload{message.RpcPayload()};

            if (cRpcPayload.empty())
            {
                mPromise.set_value();
            }
            else
            {
                try
                {
                    size_t _offset{0};
                    uint32_t _executionErrorCodeInt{
                        com::helper::ExtractInteger(cRpcPayload, _offset)};
                    auto _executionErrorCode{
                        static_cast<ExecErrc>(_executionErrorCodeInt)};
                    ExecException _exception(generateException(_executionErrorCode));
                    auto _exceptionPtr = std::make_exception_ptr(_exception);

                    mPromise.set_exception(_exceptionPtr);
                }
                catch (std::out_of_range)
                {
                    const ExecErrc cCorruptedResponseCode{
                        ExecErrc::kCommunicationError};
                    ExecException _exception(generateException(
                        cCorruptedResponseCode));
                    auto _exceptionPtr = std::make_exception_ptr(_exception);

                    mPromise.set_exception(_exceptionPtr);
                }
            }
        }

        ara::core::Result<void> ExecutionClient::ReportExecutionState(
            ExecutionState state) const
        {
            try
            {
                if (mFuture.valid())
                {
                    const ExecErrc cAlreadyReporedCode{ExecErrc::kFailed};
                    ExecException _exception(generateException(cAlreadyReporedCode));

                    throw _exception;
                }
                else
                {
                    std::vector<uint8_t> _rpcPayload;
                    mInstanceSpecifier.Serialize(_rpcPayload);

                    auto _stateByte{static_cast<uint8_t>(state)};
                    _rpcPayload.push_back(_stateByte);

                    mPromise = std::promise<void>();
                    mFuture = std::move(mPromise.get_future());
                    mRpcClient->Send(cServiceId, cMethodId, cClientId, _rpcPayload);
                    std::future_status _status{mFuture.wait_for(mTimeout)};

                    if (_status == std::future_status::timeout)
                    {
                        const ExecErrc cTimeoutCode{ExecErrc::kCommunicationError};
                        ExecException _exception(generateException(cTimeoutCode));

                        throw _exception;
                    }
                    else
                    {
                        mFuture.get();
                    }

                    core::Result<void> _result;
                    return _result;
                }
            }
            catch (const ExecException &ex)
            {
                core::Result<void> _result(ex.GetErrorCode());
                return _result;
            }
        }

        ExecutionClient::~ExecutionClient()
        {
            if (mFuture.valid())
            {
                // Wait for a timeout span until the waiting thread fetch the result before disposing
                mFuture.wait_for(mTimeout);
            }
        }
    }
}