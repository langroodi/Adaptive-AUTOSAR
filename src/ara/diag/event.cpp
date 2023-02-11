#include <stdexcept>
#include <limits>
#include <bitset>
#include "./event.h"

namespace ara
{
    namespace diag
    {
        Event::Event(const ara::core::InstanceSpecifier &specifier) : mSpecifier{specifier},
                                                                      mWirStatus{false},
                                                                      mDtcNumber{0},
                                                                      mFdc{0}
        {
            mEventStatus.encodedBits = cDefaultEventStatus;
        }

        ara::core::Result<EventStatusByte> Event::GetEventStatus()
        {
            ara::core::Result<EventStatusByte> _result{mEventStatus};
            return _result;
        }

        void Event::SetEventStatusBits(std::map<EventStatusBit, bool> statusBits)
        {
            constexpr size_t cEventStatusByteSize{sizeof(uint8_t) * 8};

            if (statusBits.empty())
            {
                return;
            }

            std::bitset<cEventStatusByteSize> _eventStatusBitSet(
                mEventStatus.encodedBits);

            for (auto statusBit : statusBits)
            {
                size_t _position;

                switch (statusBit.first)
                {
                case EventStatusBit::kTestFailed:
                    _position = 7;
                    break;
                case EventStatusBit::kTestFailedThisOperationCycle:
                    _position = 6;
                    break;
                case EventStatusBit::kTestNotCompletedThisOperationCycle:
                    _position = 1;
                    break;

                default:
                    throw std::out_of_range("The requested bit is out of range.");
                }

                _eventStatusBitSet.set(_position, statusBit.second);
            }

            const auto cNewEncodedBits{
                static_cast<uint8_t>(_eventStatusBitSet.to_ulong())};

            // Update the status and invoke the notifier (if exists) if
            // the current event status is different with the new one
            if (cNewEncodedBits != mEventStatus.encodedBits)
            {
                mEventStatus.encodedBits = cNewEncodedBits;

                if (mNotifier)
                {
                    mNotifier(mEventStatus);
                }
            }
        }

        ara::core::Result<void> Event::SetEventStatusChangedNotifier(
            std::function<void(EventStatusByte)> notifier)
        {
            mNotifier = notifier;
            ara::core::Result<void> _result;
            return _result;
        }

        ara::core::Result<bool> Event::GetLatchedWIRStatus()
        {
            ara::core::Result<bool> _result{mWirStatus};
            return _result;
        }

        ara::core::Result<void> Event::SetLatchedWIRStatus(bool status)
        {
            mWirStatus = status;
            ara::core::Result<void> _result;
            return _result;
        }

        ara::core::Result<uint32_t> Event::GetDTCNumber(DTCFormatType dtcFormat)
        {
            if (dtcFormat == DTCFormatType::kDTCFormatUDS)
            {
                ara::core::Result<uint32_t> _result{mDtcNumber};
                return _result;
            }
            else
            {
                throw std::invalid_argument(
                    "The requested DTC format is not supported.");
            }
        }

        void Event::SetDTCNumber(uint32_t dtcNumber) noexcept
        {
            mDtcNumber = dtcNumber;
        }

        ara::core::Result<DebouncingState> Event::GetDebouncingStatus()
        {
            DebouncingState _debouncingState;

            if (mFdc == std::numeric_limits<int8_t>::max())
            {
                _debouncingState = DebouncingState::kFinallyDefective;
            }
            else if (mFdc > 0)
            {
                _debouncingState = DebouncingState::kTemporarilyDefective;
            }
            else if (mFdc == std::numeric_limits<int8_t>::min())
            {
                _debouncingState = DebouncingState::kFinallyHealed;
            }
            else if (mFdc < 0)
            {
                _debouncingState = DebouncingState::kTemporarilyHealed;
            }
            else
            {
                _debouncingState = DebouncingState::kNeutral;
            }

            ara::core::Result<DebouncingState> _result{_debouncingState};
            return _result;
        }

        ara::core::Result<bool> Event::GetTestComplete()
        {
            bool _testCompleted{
                mFdc == std::numeric_limits<int8_t>::min() ||
                mFdc == std::numeric_limits<int8_t>::max()};
            ara::core::Result<bool> _result{_testCompleted};

            return _result;
        }

        ara::core::Result<int8_t> Event::GetFaultDetectionCounter()
        {
            ara::core::Result<int8_t> _result{mFdc};
            return _result;
        }

        void Event::SetFaultDetectionCounter(int8_t fdc) noexcept
        {
            mFdc = fdc;
        }
    }
}