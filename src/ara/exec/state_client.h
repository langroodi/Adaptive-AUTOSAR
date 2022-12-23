#ifndef STATE_CLIENT_H
#define STATE_CLIENT_H

#include <future>
#include "../com/someip/rpc/rpc_client.h"
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
            static const ExecErrorDomain cErrorDomain;
            const uint16_t cServiceId{3};
            const uint16_t cSetStateId{1};
            const uint16_t cStateTransition{2};
            const uint16_t cClientId{4};

            std::function<void(const ExecutionErrorEvent &)> mUndefinedStateCallback;
            com::someip::rpc::RpcClient *const mRpcClient;
            std::promise<void> mSetStatePromise;
            std::shared_future<void> mSetStateFuture;
            std::promise<void> mStateTransitionPromise;
            std::shared_future<void> mStateTransitionFuture;

            static void setPromiseException(
                std::promise<void> &promise, ExecErrc executionErrorCode);

            static void genericHandler(
                std::promise<void> &promise,
                const com::someip::rpc::SomeIpRpcMessage &message);

            void setStateHandler(
                const com::someip::rpc::SomeIpRpcMessage &message);

            void stateTransitionHandler(
                const com::someip::rpc::SomeIpRpcMessage &message);

            std::shared_future<void> getFuture(
                std::promise<void> &promise,
                uint16_t methodId,
                const std::vector<uint8_t> &rpcPayload);

        public:
            /// @brief Constructor
            /// @param undefinedStateCallback Callback to be invoked when a function group is unexpectedly terminated while state transition
            /// @param rpcClient RPC client abstraction layer
            StateClient(
                std::function<void(const ExecutionErrorEvent &)> undefinedStateCallback,
                com::someip::rpc::RpcClient *rpcClient);

            StateClient() = delete;
            ~StateClient() noexcept = default;

            /// @brief Set a function group state
            /// @param state Function group-state of interest combination
            /// @returns Void future unless an error occurs
            std::shared_future<void> SetState(const FunctionGroupState &state);

            /// @brief Push the EM to its start-up state
            /// @returns Void future unless an error occurs
            std::shared_future<void> GetInitialMachineStateTransitionResult();

            /// @brief Get the execution error made the state of function group undefined
            /// @param functionGroup Function group of interest
            /// @returns Result containing the execution error if the Undefined Function Group State exist, otherwise Failed Result
            core::Result<ExecutionErrorEvent> GetExecutionError(
                const FunctionGroup &functionGroup) noexcept;
        };
    }
}

#endif