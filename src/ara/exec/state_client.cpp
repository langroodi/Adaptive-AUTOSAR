#include "./state_client.h"

namespace ara
{
    namespace exec
    {
        const ExecErrorDomain StateClient::cErrorDomain;

        StateClient::StateClient(
            std::function<void(const ExecutionErrorEvent &)> undefinedStateCallback,
            com::someip::rpc::RpcClient *rpcClient) : mUndefinedStateCallback{undefinedStateCallback},
                                                      mRpcClient{rpcClient}
        {
            auto _setStateHandler{
                std::bind(
                    &StateClient::setStateHandler,
                    this, std::placeholders::_1)};
            mRpcClient->SetHandler(
                cServiceId, cSetStateId, _setStateHandler);

            auto _stateTransitionHandler{
                std::bind(
                    &StateClient::stateTransitionHandler,
                    this, std::placeholders::_1)};
            mRpcClient->SetHandler(
                cServiceId, cStateTransition, _stateTransitionHandler);
        }

        void StateClient::setPromiseException(
            std::promise<void> &promise, ExecErrc executionErrorCode)
        {
            auto _errorValue{static_cast<core::ErrorDomain::CodeType>(executionErrorCode)};
            core::ErrorCode _errorCode{_errorValue, cErrorDomain};
            ExecException _exception{_errorCode};
            auto _exceptionPtr = std::make_exception_ptr(_exception);
            promise.set_exception(_exceptionPtr);
        }

        void StateClient::genericHandler(
            std::promise<void> &promise,
            const com::someip::rpc::SomeIpRpcMessage &message)
        {
            const std::vector<u_int8_t> &cRpcPayload{message.RpcPayload()};

            if (cRpcPayload.empty())
            {
                promise.set_value();
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

                    setPromiseException(promise, _executionErrorCode);
                }
                catch (std::out_of_range)
                {
                    const ExecErrc cCorruptedResponseCode{
                        ExecErrc::kCommunicationError};

                    setPromiseException(promise, cCorruptedResponseCode);
                }
            }
        }

        void StateClient::setStateHandler(
            const com::someip::rpc::SomeIpRpcMessage &message)
        {
            genericHandler(mSetStatePromise, message);
        }

        void StateClient::stateTransitionHandler(
            const com::someip::rpc::SomeIpRpcMessage &message)
        {
            genericHandler(mStateTransitionPromise, message);
        }

        std::shared_future<void> StateClient::getFuture(
            std::promise<void> &promise,
            uint16_t methodId,
            const std::vector<uint8_t> &rpcPayload)
        {
            try
            {
                std::shared_future<void> _result{promise.get_future()};
                mRpcClient->Send(cServiceId, methodId, cClientId, rpcPayload);

                return _result;
            }
            catch (std::future_error)
            {
                const ExecErrc cAlreadyRequestedCode{ExecErrc::kFailed};
                std::promise<void> _promise;
                setPromiseException(_promise, cAlreadyRequestedCode);
                std::shared_future<void> _result{_promise.get_future()};

                return _result;
            }
        }

        std::shared_future<void> StateClient::SetState(
            const FunctionGroupState &state)
        {
            if (mSetStateFuture.valid())
            {
                try
                {
                    const ExecErrc cExecErrc{ExecErrc::kCancelled};
                    setPromiseException(mSetStatePromise, cExecErrc);
                }
                catch (std::future_error)
                {
                    // Catch the exception if the promise is already set
                }

                mSetStatePromise = std::promise<void>();
            }

            std::vector<uint8_t> _rpcPayload;
            state.Serialize(_rpcPayload);
            mSetStateFuture =
                getFuture(mSetStatePromise, cSetStateId, _rpcPayload);

            return mSetStateFuture;
        }

        std::shared_future<void> StateClient::GetInitialMachineStateTransitionResult()
        {
            if (mStateTransitionFuture.valid())
            {
                try
                {
                    const ExecErrc cExecErrc{ExecErrc::kCancelled};
                    setPromiseException(mStateTransitionPromise, cExecErrc);
                }
                catch (std::future_error)
                {
                    // Catch the exception if the promise is already set
                }

                mStateTransitionPromise = std::promise<void>();
            }

            const std::vector<uint8_t> cRpcPayload;
            mStateTransitionFuture =
                getFuture(mStateTransitionPromise, cStateTransition, cRpcPayload);

            return mStateTransitionFuture;
        }

        core::Result<ExecutionErrorEvent> StateClient::GetExecutionError(
            const FunctionGroup &functionGroup) noexcept
        {
            const ExecErrc cExecErrc{ExecErrc::kFailed};
            auto _errorValue{static_cast<core::ErrorDomain::CodeType>(cExecErrc)};
            core::ErrorCode _errorCode{_errorValue, cErrorDomain};
            core::Result<ExecutionErrorEvent> _result{_errorCode};

            return _result;
        }
    }
}