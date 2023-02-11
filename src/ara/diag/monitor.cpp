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
                                                                  mOffered{false},
                                                                  mEvent{nullptr}
        {
        }

        void Monitor::onEventStatusChanged(bool passed)
        {
            const int8_t cFailedFdc{127};
            const int8_t cPassedFdc{-128};
            const bool cTestNotCompleted{false};

            if (mEvent)
            {
                mEvent->SetFaultDetectionCounter(passed ? cPassedFdc : cFailedFdc);
                mEvent->SetEventStatusBits(
                    {{EventStatusBit::kTestFailed, !passed},
                     {EventStatusBit::kTestNotCompletedThisOperationCycle, cTestNotCompleted}});
            }
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

                case MonitorAction::kResetTestFailed:
                    if (mEvent)
                    {
                        mEvent->SetEventStatusBits({{EventStatusBit::kTestFailed, false}});
                    }
                    break;

                default:
                    throw std::invalid_argument("Reported monitor action is not supported.");
                }
            }
        }

        void Monitor::AttachEvent(Event *event)
        {
            mEvent = event;
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
                if (mInitMonitor)
                {
                    mInitMonitor(InitMonitorReason::kReenabled);
                }

                return _result;
            }
        }

        void Monitor::StopOffer()
        {
            if (mOffered)
            {
                mOffered = false;

                if (mInitMonitor)
                {
                    mInitMonitor(InitMonitorReason::kDisabled);
                }
            }
        }

        Monitor::~Monitor() noexcept
        {
            delete mDebouncer;
        }
    }
}