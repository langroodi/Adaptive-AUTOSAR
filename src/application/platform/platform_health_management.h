#ifndef PLATFORM_HEALTH_MANAGEMENT_H
#define PLATFORM_HEALTH_MANAGEMENT_H

#include <set>
#include "../../arxml/arxml_reader.h"
#include "../../ara/exec/helper/modelled_process.h"
#include "../../ara/phm/checkpoint_communicator.h"
#include "../../ara/phm/supervisors/alive_supervision.h"
#include "../../ara/phm/supervisors/deadline_supervision.h"
#include "../../ara/phm/supervisors/global_supervision.h"
#include "../helper/log_recovery_action.h"

namespace application
{
    namespace platform
    {
        /// @brief Platform health managment modelled process
        class PlatformHealthManagement : public ara::exec::helper::ModelledProcess
        {
        private:
            static const std::string cAppId;
            const ara::exec::FunctionGroup cFunctionGroup;

            ara::phm::CheckpointCommunicator *const mCheckpointCommunicator;
            ara::phm::supervisors::AliveSupervision *mAliveSupervision;
            ara::phm::supervisors::DeadlineSupervision *mDeadlineSupervision;
            ara::phm::supervisors::GlobalSupervision *mGlobalSupervision;
            helper::LogRecoveryAction mRecoveryAction;
            std::map<uint32_t, std::function<void()>> mReportDelegates;

            static uint32_t getCheckpointId(const std::string& content);
            static void fillCheckpoints(
                const arxml::ArxmlReader &reader,
                std::set<uint32_t> &checkpoints);

            void onReportCheckpoint(uint32_t checkpoint);
            bool tryAddReportDelegate(
                std::string checkpointIdStr,
                const std::set<uint32_t> &checkpoints,
                std::function<void()> &&delegate);
            void configureAliveSupervision(
                const arxml::ArxmlReader &reader,
                const std::set<uint32_t> &checkpoints);
            void configureDeadlineSupervision(
                const arxml::ArxmlReader &reader,
                const std::set<uint32_t> &checkpoints);
            void onGlobalStatusChanged(
                ara::phm::supervisors::SupervisionUpdate update);

        protected:
            int Main(
                const std::atomic_bool *cancellationToken,
                const std::map<std::string, std::string> &arguments) override;

        public:
            /// @brief Constructor
            /// @param poller Global poller for network communication
            /// @param checkpointCommunicator Medium to communicate the supervision checkpoints
            /// @param functionGroup Function group name monitored by the PHM
            PlatformHealthManagement(
                AsyncBsdSocketLib::Poller *poller,
                ara::phm::CheckpointCommunicator *checkpointCommunicator,
                std::string functionGroup);

            ~PlatformHealthManagement() override;
        };
    }
}

#endif