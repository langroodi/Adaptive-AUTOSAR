#ifndef STATE_CLIENT_H
#define STATE_CLIENT_H

#include <future>
#include <mutex>
#include "./helper/fifo_layer.h"
#include "./execution_error_event.h"
#include "./function_group_state.h"
#include "./exec_exception.h"

namespace ara
{
    namespace exec
    {
        /// @brief Class the enables State Management to interact with Execution Management
        class StateClient final
        {
        private:
            using SetStateMessage = std::pair<const FunctionGroup *, std::string>;

            static const ExecErrorDomain cErrorDomain;
            std::function<void(const ExecutionErrorEvent &)> mUndefinedStateCallback;
            helper::FifoLayer<SetStateMessage> *mCommunicationLayer;
            std::mutex mInitializationMutex;
            bool mInitialized;

            void setPromiseException(
                std::promise<void>& promise, ExecErrc executionErrorCode) const;

        public:
            /// @brief Constructor
            /// @param undefinedStateCallback Callback to be invoked when a function group is unexpectedly terminated while state transition
            /// @param communicationLayer FIFO IPC abstraction layer
            StateClient(
                std::function<void(const ExecutionErrorEvent &)> undefinedStateCallback,
                helper::FifoLayer<SetStateMessage> *communicationLayer);

            StateClient() = delete;
            ~StateClient() noexcept = default;

            /// @brief Set a function group state
            /// @param state Function group-state of interest combination
            /// @returns Void future unless an error occurs
            std::future<void> SetState(const FunctionGroupState &state);

            /// @brief Push the EM to its start-up state
            /// @returns Void future unless an error occurs
            std::future<void> GetInitialMachineStateTransitionResult();

            /// @brief Get the execution error made the state of function group undefined
            /// @param functionGroup Function group of interest
            /// @returns Result containing the execution error if the Undefined Function Group State exist, otherwise Failed Result
            core::Result<ExecutionErrorEvent> GetExecutionError(
                const FunctionGroup &functionGroup) noexcept;
        };
    }
}

#endif