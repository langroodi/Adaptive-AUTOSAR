#include "./state_client.h"

namespace ara
{
    namespace exec
    {
        const ExecErrorDomain StateClient::cErrorDomain;

        StateClient::StateClient(
            std::function<void(const ExecutionErrorEvent &)> undefinedStateCallback,
            helper::FifoLayer<SetStateMessage> *communicationLayer) : mUndefinedStateCallback{undefinedStateCallback},
                                                                      mCommunicationLayer{communicationLayer},
                                                                      mInitialized{false}
        {
        }

        std::future<void> StateClient::SetState(
            const FunctionGroupState &state)
        {
            std::promise<void> _promise;
            std::future<void> _result{_promise.get_future()};
            const FunctionGroup *_functionGroup = &state.GetFunctionGroup();
            std::string _state = state.GetState();
            SetStateMessage _message{std::make_pair(_functionGroup, _state)};
            mCommunicationLayer->Send(_message);
            _promise.set_value();

            return _result;
        }

        void StateClient::setPromiseException(
            std::promise<void> &promise, ExecErrc executionErrorCode) const
        {
            auto _errorValue{static_cast<core::ErrorDomain::CodeType>(executionErrorCode)};
            core::ErrorCode _errorCode{_errorValue, cErrorDomain};
            ExecException _exception{_errorCode};
            auto _exceptionPtr = std::make_exception_ptr(_exception);
            promise.set_exception(_exceptionPtr);
        }

        std::future<void> StateClient::GetInitialMachineStateTransitionResult()
        {
            std::unique_lock<std::mutex> _lock{mInitializationMutex, std::defer_lock};
            std::promise<void> _promise;
            std::future<void> _result{_promise.get_future()};

            if (mInitialized)
            {
                // EM reinitialization shouldn't be possible:
                setPromiseException(_promise, ExecErrc::kFailed);
            }
            else if (_lock.try_lock())
            {
                // EM is not intialized yet:
                mInitialized = true;
                _promise.set_value();
            }
            else
            {
                // EM is initializing, so ignore the new request:
                setPromiseException(_promise, ExecErrc::kGeneralError);
            }

            return _result;
        }

        core::Result<ExecutionErrorEvent> StateClient::GetExecutionError(
            const FunctionGroup &functionGroup) noexcept
        {
            /// @todo Implement set state tracking
            const ExecErrc cExecErrc{ExecErrc::kFailed};
            auto _errorValue{static_cast<core::ErrorDomain::CodeType>(cExecErrc)};
            core::ErrorCode _errorCode{_errorValue, cErrorDomain};
            core::Result<ExecutionErrorEvent> _result{_errorCode};

            return _result;
        }
    }
}