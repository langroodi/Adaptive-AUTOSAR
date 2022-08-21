#include "./monitor.h"
#include "./diag_error_domain.h"

namespace ara
{
    namespace diag
    {
        Monitor::Monitor(
            const core::InstanceSpecifier &specifier,
            std::function<void(InitMonitorReason)> initMonitor) : mSpecifier{specifier},
                                                                  mInitMonitor{initMonitor},
                                                                  mOffered{false}
        {
        }

        void Monitor::onEventStatusChanged(bool passed)
        {
            /// @todo Update the corresponding diagnostic event
        }

        Monitor::Monitor(
            const core::InstanceSpecifier &specifier,
            std::function<void(InitMonitorReason)> initMonitor,
            CounterBased defaultValues) : Monitor(specifier, initMonitor)
        {
            auto _callback{
                std::bind(
                    &Monitor::onEventStatusChanged, this, std::placeholders::_1)};

            mDebouncer =
                new debouncing::CounterBasedDebouncer(_callback, defaultValues);
        }

        Monitor::Monitor(
            const core::InstanceSpecifier &specifier,
            std::function<void(InitMonitorReason)> initMonitor,
            TimeBased defaultValues) : Monitor(specifier, initMonitor)
        {
            auto _callback{
                std::bind(
                    &Monitor::onEventStatusChanged, this, std::placeholders::_1)};

            mDebouncer =
                new debouncing::TimerBasedDebouncer(_callback, defaultValues);
        }

        void Monitor::ReportMonitorAction(MonitorAction action)
        {
            if (mOffered)
            {
                switch (action)
                {
                case MonitorAction::kPassed:
                    mDebouncer->ReportPassed();
                    break;

                case MonitorAction::kFailed:
                    mDebouncer->ReportFailed();
                    break;

                case MonitorAction::kPrepassed:
                    mDebouncer->ReportPrepassed();
                    break;

                case MonitorAction::kPrefailed:
                    mDebouncer->ReportPrefailed();
                    break;

                case MonitorAction::kFreezeDebouncing:
                    mDebouncer->Freeze();
                    break;

                case MonitorAction::kResetDebouncing:
                    mDebouncer->Reset();
                    break;

                default:
                    /// @todo Consider other monitor actions
                    break;
                }
            }
        }

        core::Result<void> Monitor::Offer()
        {
            if (mOffered)
            {
                core::ErrorDomain *_errorDomain{DiagErrorDomain::GetDiagDomain()};
                auto _diagErrorDomain{static_cast<DiagErrorDomain *>(_errorDomain)};
                core::ErrorCode _errorCode{_diagErrorDomain->MakeErrorCode(DiagErrc::kAlreadyOffered)};
                auto _result{core::Result<void>::FromError(_errorCode)};

                return _result;
            }
            else
            {
                mOffered = true;
                core::Result<void> _result;

                return _result;
            }
        }

        void Monitor::StopOffer()
        {
            mOffered = false;
        }

        Monitor::~Monitor() noexcept
        {
            delete mDebouncer;
        }
    }
}