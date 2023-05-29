#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include "../../ara/com/someip/rpc/socket_rpc_client.h"
#include "../../ara/exec/helper/modelled_process.h"
#include "../../ara/exec/deterministic_client.h"
#include "../../ara/exec/state_client.h"
#include "../helper/rpc_configuration.h"

namespace application
{
    namespace platform
    {
        /// @brief State managment modelled process
        class StateManagement : public ara::exec::helper::ModelledProcess
        {
        private:
            static const std::string cAppId;

            std::vector<ara::exec::FunctionGroup> mFunctionGroups;
            std::vector<ara::exec::FunctionGroupState> mFunctionGroupStates;
            ara::core::InstanceSpecifier mInstanceSpecifier;

            static helper::RpcConfiguration getRpcConfiguration(
                const std::string &configFilepath);

            void configureStates(
                const ara::exec::FunctionGroup &functionGroup,
                std::string &&functionGroupContent);

            void configureFunctionGroups(const std::string &configFilepath);

            void onUndefinedState(const ara::exec::ExecutionErrorEvent &event);

            void reportExecutionState(
                ara::com::someip::rpc::RpcClient *rpcClient);

            std::shared_future<void> transitToStartUpState(
                ara::exec::StateClient &stateClient);

            template <class Future>
            void checkFuture(Future &future, std::string &&message)
            {
                const std::chrono::seconds cDuration{0};

                if (future.valid())
                {
                    try
                    {
                        std::future_status _status{future.wait_for(cDuration)};

                        if (_status == std::future_status::ready)
                        {
                            future.get();
                            future = Future{};

                            ara::log::LogStream _logStream;
                            _logStream << message;
                            Log(cLogLevel, _logStream);
                        }
                    }
                    catch (const ara::exec::ExecException &ex)
                    {
                        throw std::runtime_error(ex.what());
                    }
                }
            }

        protected:
            int Main(
                const std::atomic_bool *cancellationToken,
                const std::map<std::string, std::string> &arguments) override;

        public:
            /// @brief Constructor
            /// @param poller Global poller for network communication
            explicit StateManagement(AsyncBsdSocketLib::Poller *poller);
        };
    }
}

#endif