#include "../helper/argument_configuration.h"
#include "./platform_health_management.h"

namespace application
{
    namespace platform
    {
        const std::string PlatformHealthManagement::cAppId{"PlatformHeathManagement"};

        PlatformHealthManagement::PlatformHealthManagement(
            AsyncBsdSocketLib::Poller *poller,
            ara::phm::CheckpointCommunicator *checkpointCommunicator,
            std::string functionGroup) : ara::exec::helper::ModelledProcess(cAppId, poller),
                                         mCheckpointCommunicator{checkpointCommunicator},
                                         cFunctionGroup{ara::exec::FunctionGroup::Create(functionGroup).Value()},
                                         mAliveSupervision{nullptr},
                                         mDeadlineSupervision{nullptr},
                                         mGlobalSupervision{nullptr}
        {
            auto _onReportCheckpoint{
                std::bind(
                    &PlatformHealthManagement::onReportCheckpoint,
                    this, std::placeholders::_1)};
            mCheckpointCommunicator->SetCallback(std::move(_onReportCheckpoint));
        }

        void PlatformHealthManagement::onReportCheckpoint(uint32_t checkpoint)
        {
            auto _reportDelegateItr{mReportDelegates.find(checkpoint)};

            if (_reportDelegateItr != mReportDelegates.end())
            {
                _reportDelegateItr->second();
            }
        }

        uint32_t PlatformHealthManagement::getCheckpointId(const std::string &content)
        {
            const arxml::ArxmlReader cCheckpointReader(
                content.c_str(), content.length());

            const arxml::ArxmlNode cCheckpointIdNode{
                cCheckpointReader.GetRootNode(
                    {"SUPERVISION-CHECKPOINT", "CHECKPOINT-ID"})};

            const auto cResult{cCheckpointIdNode.GetValue<uint32_t>()};
            return cResult;
        }

        void PlatformHealthManagement::fillCheckpoints(
            const arxml::ArxmlReader &reader,
            std::set<uint32_t> &checkpoints)
        {
            const arxml::ArxmlNodeRange cCheckpointNodes{
                reader.GetNodes(
                    {"AUTOSAR",
                     "AR-PACKAGES",
                     "AR-PACKAGE",
                     "ELEMENTS",
                     "GLOBAL-SUPERVISION",
                     "CHECKPOINTS"})};

            for (const auto cCheckpointNode : cCheckpointNodes)
            {
                const uint32_t cCheckpointId{
                    getCheckpointId(cCheckpointNode.GetContent())};
                checkpoints.insert(cCheckpointId);
            }
        }

        bool PlatformHealthManagement::tryAddReportDelegate(
            std::string checkpointIdStr,
            const std::set<uint32_t> &checkpoints,
            std::function<void()> &&delegate)
        {
            const auto cCheckpointId{
                static_cast<uint32_t>(std::stoul(checkpointIdStr))};

            auto _checkpointItr{checkpoints.find(cCheckpointId)};
            if (_checkpointItr != checkpoints.cend())
            {
                auto _pair{std::make_pair(cCheckpointId, std::move(delegate))};
                mReportDelegates.insert(_pair);

                return true;
            }
            else
            {
                return false;
            }
        }

        void PlatformHealthManagement::configureAliveSupervision(
            const arxml::ArxmlReader &reader,
            const std::set<uint32_t> &checkpoints)
        {
            const arxml::ArxmlNode cAliveSupervisionNode{
                reader.GetRootNode(
                    {"AUTOSAR",
                     "AR-PACKAGES",
                     "AR-PACKAGE",
                     "ELEMENTS",
                     "GLOBAL-SUPERVISION",
                     "ALIVE-SUPERVISION"})};

            const std::string cAliveSupervisionContent{
                cAliveSupervisionNode.GetContent()};
            const arxml::ArxmlReader cAliveSupervisionReader(
                cAliveSupervisionContent.c_str(),
                cAliveSupervisionContent.length());

            const auto cExpectedAliveIndications{
                cAliveSupervisionReader.GetRootNode(
                                           {"ALIVE-SUPERVISION", "EXPECTED-ALIVE-INDICATIONS"})
                    .GetValue<uint16_t>()};

            const auto cMinMargin{
                cAliveSupervisionReader.GetRootNode(
                                           {"ALIVE-SUPERVISION", "MIN-MARGIN"})
                    .GetValue<uint16_t>()};

            const auto cMaxMargin{
                cAliveSupervisionReader.GetRootNode(
                                           {"ALIVE-SUPERVISION", "MAX-MARGIN"})
                    .GetValue<uint16_t>()};

            const auto cAliveReferenceCycleUInt{
                cAliveSupervisionReader.GetRootNode(
                                           {"ALIVE-SUPERVISION", "ALIVE-REFERENCE-CYCLE"})
                    .GetValue<uint32_t>()};
            const std::chrono::milliseconds cAliveReferenceCycle(
                cAliveReferenceCycleUInt);

            const auto cFailedReferenceCyclesTolerance{
                cAliveSupervisionReader.GetRootNode(
                                           {"ALIVE-SUPERVISION", "FAILED-REFERENCE-CYCLES-TOLERANCE"})
                    .GetValue<uint8_t>()};

            mAliveSupervision =
                new ara::phm::supervisors::AliveSupervision(
                    cExpectedAliveIndications,
                    cMinMargin,
                    cMaxMargin,
                    cAliveReferenceCycle,
                    cFailedReferenceCyclesTolerance);

            std::string _checkpointIdStr;
            bool _successful{
                cAliveSupervisionNode.TryGetReference(
                    "CHECKPOINT-REF",
                    "SUPERVISION-CHECKPOINT",
                    _checkpointIdStr)};

            if (_successful)
            {
                auto _delegate{
                    std::bind(
                        &ara::phm::supervisors::AliveSupervision::ReportAliveness,
                        mAliveSupervision)};
                _successful =
                    tryAddReportDelegate(
                        _checkpointIdStr, checkpoints, std::move(_delegate));

                if (!_successful)
                {
                    throw std::runtime_error(
                        "Alive supervision refers to an inavlid checkpoint!");
                }
            }
            else
            {
                throw std::runtime_error(
                    "Alive supervision checkpoint reference is corrupted!");
            }
        }

        void PlatformHealthManagement::configureDeadlineSupervision(
            const arxml::ArxmlReader &reader,
            const std::set<uint32_t> &checkpoints)
        {
            const arxml::ArxmlNode cDeadlineSupervisionNode{
                reader.GetRootNode(
                    {"AUTOSAR",
                     "AR-PACKAGES",
                     "AR-PACKAGE",
                     "ELEMENTS",
                     "GLOBAL-SUPERVISION",
                     "DEADLINE-SUPERVISION"})};

            const std::string cDeadlineSupervisionContent{
                cDeadlineSupervisionNode.GetContent()};
            const arxml::ArxmlReader cDeadlineSupervisionReader(
                cDeadlineSupervisionContent.c_str(),
                cDeadlineSupervisionContent.length());

            const auto cMinDeadlineUInt{
                cDeadlineSupervisionReader.GetRootNode(
                                              {"DEADLINE-SUPERVISION", "MIN-DEADLINE"})
                    .GetValue<uint32_t>()};
            const std::chrono::milliseconds cMinDeadline(cMinDeadlineUInt);

            const auto cMaxDeadlineUInt{
                cDeadlineSupervisionReader.GetRootNode(
                                              {"DEADLINE-SUPERVISION", "MAX-DEADLINE"})
                    .GetValue<uint32_t>()};
            const std::chrono::milliseconds cMaxDeadline(cMaxDeadlineUInt);

            mDeadlineSupervision =
                new ara::phm::supervisors::DeadlineSupervision(
                    cMinDeadline, cMaxDeadline);

            const arxml::ArxmlNode cCheckpointTransition{
                cDeadlineSupervisionReader.GetRootNode(
                    {"DEADLINE-SUPERVISION", "TRANSITION", "CHECKPOINT-TRANSITION"})};

            std::string _sourceCheckpointIdStr;
            bool _successful{
                cCheckpointTransition.TryGetReference(
                    "SOURCE-REF",
                    "SUPERVISION-CHECKPOINT",
                    _sourceCheckpointIdStr)};

            if (_successful)
            {
                auto _delegate{
                    std::bind(
                        &ara::phm::supervisors::DeadlineSupervision::ReportSourceCheckpoint,
                        mDeadlineSupervision)};
                _successful =
                    tryAddReportDelegate(
                        _sourceCheckpointIdStr, checkpoints, std::move(_delegate));

                if (!_successful)
                {
                    throw std::runtime_error(
                        "Deadline supervision refers to an inavlid source checkpoint!");
                }
            }
            else
            {
                throw std::runtime_error(
                    "Deadline supervision source checkpoint reference is corrupted!");
            }

            std::string _targetCheckpointIdStr;
            _successful =
                cCheckpointTransition.TryGetReference(
                    "TARGET-REF",
                    "SUPERVISION-CHECKPOINT",
                    _targetCheckpointIdStr);

            if (_successful)
            {
                auto _delegate{
                    std::bind(
                        &ara::phm::supervisors::DeadlineSupervision::ReportTargetCheckpoint,
                        mDeadlineSupervision)};
                _successful =
                    tryAddReportDelegate(
                        _targetCheckpointIdStr, checkpoints, std::move(_delegate));

                if (!_successful)
                {
                    throw std::runtime_error(
                        "Deadline supervision refers to an inavlid target checkpoint!");
                }
            }
            else
            {
                throw std::runtime_error(
                    "Deadline supervision target checkpoint reference is corrupted!");
            }
        }

        void PlatformHealthManagement::onGlobalStatusChanged(
            ara::phm::supervisors::SupervisionUpdate update)
        {
            if (update.status == ara::phm::supervisors::SupervisionStatus::kExpired)
            {
                const ara::exec::ExecutionError cSupervisionExpired{0};

                ara::exec::ExecutionErrorEvent _errorEvent;
                _errorEvent.functionGroup = &cFunctionGroup;
                _errorEvent.executionError = cSupervisionExpired;

                mRecoveryAction.RecoveryHandler(_errorEvent, update.type);
            }
        }

        int PlatformHealthManagement::Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments)
        {
            const std::string cConfigArgument{
                helper::ArgumentConfiguration::cPhmConfigArgument};

            ara::log::LogStream _logStream;

            try
            {
                const std::string cConfigFilepath{arguments.at(cConfigArgument)};
                const arxml::ArxmlReader cArxmlReader(cConfigFilepath);

                std::set<uint32_t> _checkpoints;
                fillCheckpoints(cArxmlReader, _checkpoints);

                configureAliveSupervision(cArxmlReader, _checkpoints);
                configureDeadlineSupervision(cArxmlReader, _checkpoints);

                mRecoveryAction.Offer();
                mGlobalSupervision =
                    new ara::phm::supervisors::GlobalSupervision(
                        {mAliveSupervision, mDeadlineSupervision});

                auto _onGlobalStatusChanged{
                    std::bind(
                        &PlatformHealthManagement::onGlobalStatusChanged,
                        this, std::placeholders::_1)};
                mGlobalSupervision->SetCallback(_onGlobalStatusChanged);

                _logStream << "Plafrom health management has been initialized.";
                Log(cLogLevel, _logStream);

                bool _running{true};

                while (!cancellationToken->load() && _running)
                {
                    _running = WaitForActivation();
                }

                mCheckpointCommunicator->ResetCallback();
                _logStream.Flush();
                _logStream << "Plafrom health management has been terminated.";
                Log(cLogLevel, _logStream);

                return cSuccessfulExitCode;
            }
            catch (const std::runtime_error &ex)
            {
                _logStream.Flush();
                _logStream << ex.what();
                Log(cErrorLevel, _logStream);

                return cUnsuccessfulExitCode;
            }
        }

        PlatformHealthManagement::~PlatformHealthManagement()
        {
            mCheckpointCommunicator->ResetCallback();

            if (mGlobalSupervision)
            {
                delete mGlobalSupervision;
            }

            if (mDeadlineSupervision)
            {
                delete mDeadlineSupervision;
            }

            if (mAliveSupervision)
            {
                delete mAliveSupervision;
            }
        }
    }
}