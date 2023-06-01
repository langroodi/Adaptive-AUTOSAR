#ifndef EXECUTION_MANAGEMENT_H
#define EXECUTION_MANAGEMENT_H

#include <set>
#include "../../ara/exec/state_server.h"
#include "../helper/fifo_checkpoint_communicator.h"
#include "../extended_vehicle.h"
#include "./state_management.h"
#include "./platform_health_management.h"
#include "./diagnostic_manager.h"

/// @brief AUTOSAR application namespace
namespace application
{
    /// @brief AUTOSAR platform application namespace
    namespace platform
    {
        /// @brief Execution managment modelled process
        class ExecutionManagement : public ara::exec::helper::ModelledProcess
        {
        private:
            static const std::string cAppId;
            static const std::string cFifoPath;
            const std::string cMachineFunctionGroup{"MachineFG"};

            helper::FifoCheckpointCommunicator mCommunicator;
            StateManagement mStateManagement;
            PlatformHealthManagement mPlatformHealthManager;
            ExtendedVehicle mExtendedVehicle;
            DiagnosticManager mDiagnosticManager;
            ara::exec::StateServer *mStateServer;

            static helper::RpcConfiguration getRpcConfiguration(
                const std::string &configFilepath);

            static void fillFunctionGroupStates(
                std::string functionGroupShortName,
                const std::string &functionGroupContent,
                std::set<std::pair<std::string, std::string>> &functionGroupStates);

            static void fillInitialStates(
                std::string functionGroupShortName,
                const std::string &functionGroupContent,
                std::map<std::string, std::string> &initialStates);

            static void fillStates(
                const std::string &configFilepath,
                std::set<std::pair<std::string, std::string>> &functionGroupStates,
                std::map<std::string, std::string> &initialStates);

            void onStateChange(
                const std::map<std::string, std::string> &arguments);

        protected:
            int Main(
                const std::atomic_bool *cancellationToken,
                const std::map<std::string, std::string> &arguments) override;

        public:
            /// @brief Constructor
            /// @param poller Global poller for network communication
            explicit ExecutionManagement(AsyncBsdSocketLib::Poller *poller);

            ~ExecutionManagement() override;
        };
    }
}

#endif