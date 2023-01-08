#ifndef STATE_SERVER_H
#define STATE_SERVER_H

#include <atomic>
#include <map>
#include <mutex>
#include <set>
#include "../com/someip/rpc/rpc_server.h"
#include "../sm/trigger.h"
#include "./exec_exception.h"

namespace ara
{
    namespace exec
    {
        /// @brief A class to handle the state client requests at the EM side
        /// @see StateClient
        /// @note The class is not part of the ARA standard.
        class StateServer
        {
        private:
            const uint16_t cServiceId{3};
            const uint16_t cSetStateId{1};
            const uint16_t cStateTransitionId{2};

            com::someip::rpc::RpcServer *const mRpcServer;
            const std::set<std::pair<std::string, std::string>> mFunctionGroupStates;
            std::map<std::string, sm::Trigger<std::string> *> mNotifiers;
            std::map<std::string, std::string> mCurrentStates;
            std::atomic_bool mInitialized;
            std::mutex mMutex;

            void injectErrorCode(
                std::vector<uint8_t> &payload, ExecErrc errorCode);

            void notify(std::string functionGroup, std::string state);

            bool handleSetState(
                const std::vector<uint8_t> &rpcRequestPayload,
                std::vector<uint8_t> &rpcResponsePayload);

            bool handleStateTransition(
                const std::vector<uint8_t> &rpcRequestPayload,
                std::vector<uint8_t> &rpcResponsePayload);

        public:
            /// @brief Constructor
            /// @param rpcServer RPC server abstraction layer
            /// @param functionGroupStates Function groups and their possible states combination
            /// @param initialStates Initial states of the function groups
            /// @throws std::invalid_argument Throws when an initial state refers to an invalid function group
            /// @throws std::logic_error Throws when a function group lacks of the initial state
            StateServer(
                com::someip::rpc::RpcServer *rpcServer,
                std::set<std::pair<std::string, std::string>> &&functionGroupStates,
                std::map<std::string, std::string> &&initialStates);

            /// @brief Try to get the state of a function group
            /// @param[in] functionGroup Function group name
            /// @param[out] state Function group state
            /// @return True if the function group exits; otherwise false
            /// @remark The function is thread-safe.
            bool TryGetState(
                std::string functionGroup, std::string &state);

            /// @brief Set a notifier at the state changed of a function group
            /// @param functionGroup Function group of interest
            /// @param callback Callback to be invoked at the state change
            /// @throws std::out_of_range Throws when the function group does not exist
            void SetNotifier(
                std::string functionGroup, std::function<void()> callback);

            /// @brief Indicate whether or not EM has been initialized
            /// @return True if EM is initialized; otherwise false
            /// @remark The function call is atomic.
            bool Initialized() const noexcept;

            ~StateServer();
        };
    }
}

#endif