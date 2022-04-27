#include "./dtc_information.h"

namespace ara
{
    namespace diag
    {
        DTCInformation::DTCInformation(const ara::core::InstanceSpecifier &specifier) : mSpecifier{specifier},
                                                                                        mControlDtcStatus{ControlDtcStatusType::kDTCSettingOff}
        {
        }

        ara::core::Result<UdsDtcStatusByteType> DTCInformation::GetCurrentStatus(uint32_t dtc)
        {
            /// @todo In case of out_of_range exception return kNoSuchDtc (108) error code
            UdsDtcStatusByteType _status{mStatuses.at(dtc)};
            ara::core::Result<UdsDtcStatusByteType> _result{_status};

            return _result;
        }

        void DTCInformation::SetCurrentStatus(
            uint32_t dtc, UdsDtcStatusBitType mask, UdsDtcStatusByteType status)
        {
            auto _iterator{mStatuses.find(dtc)};

            if (_iterator == mStatuses.end())
            {
                // Add the DTC status
                mStatuses[dtc] = status;

                if (mNumberOfStoredEntriesNotifier)
                {
                    auto _size{static_cast<uint32_t>(mStatuses.size())};
                    mNumberOfStoredEntriesNotifier(_size);
                }
            }
            else
            {
                // Edit the DTC status
                UdsDtcStatusByteType _currentStatus{_iterator->second};
                auto _differenceByte{
                    static_cast<uint8_t>(
                        _currentStatus.encodedBits ^ status.encodedBits)};

                auto _maskByte{static_cast<uint8_t>(mask)};
                auto _maskedDifferenceByte{
                    static_cast<uint8_t>(_differenceByte & _maskByte)};

                // Edit the status byte if there is any difference
                if (_maskedDifferenceByte)
                {
                    // Flip the current status bits
                    auto _newStatusBits{
                        static_cast<uint8_t>(
                            _currentStatus.encodedBits ^ _maskedDifferenceByte)};

                    _iterator->second.encodedBits = _newStatusBits;

                    if (mDtcStatusChangedNotifier)
                    {
                        mDtcStatusChangedNotifier(
                            dtc, _currentStatus, _iterator->second);
                    }
                }
            }
        }

        ara::core::Result<void> DTCInformation::SetDTCStatusChangedNotifier(
            std::function<void(uint32_t, UdsDtcStatusByteType, UdsDtcStatusByteType)> notifier)
        {
            // In contrast with the ARA standard, no evaluation is perfomed to validate the notifier pointer.
            mDtcStatusChangedNotifier = notifier;
            ara::core::Result<void> _result;

            return _result;
        }

        ara::core::Result<uint32_t> DTCInformation::GetNumberOfStoredEntries()
        {
            auto _size{static_cast<uint32_t>(mStatuses.size())};
            ara::core::Result<uint32_t> _result{_size};

            return _result;
        }

        ara::core::Result<void> DTCInformation::SetNumberOfStoredEntriesNotifier(
            std::function<void(uint32_t)> notifier)
        {
            // In contrast with the ARA standard, no evaluation is perfomed to validate the notifier pointer.
            mNumberOfStoredEntriesNotifier = notifier;
            ara::core::Result<void> _result;

            return _result;
        }

        ara::core::Result<void> DTCInformation::Clear(uint32_t dtc)
        {
            ara::core::Result<void> _result;
            auto _iterator{mStatuses.find(dtc)};

            if (_iterator != mStatuses.end())
            {
                mStatuses.erase(_iterator);
                if (mNumberOfStoredEntriesNotifier)
                {
                    mNumberOfStoredEntriesNotifier(mStatuses.size());
                }
            }
            else
            {
                /// @todo Return kWrong error code
            }

            return _result;
        }

        ara::core::Result<ControlDtcStatusType> DTCInformation::GetControlDTCStatus()
        {
            ara::core::Result<ControlDtcStatusType> _result{mControlDtcStatus};
            return _result;
        }

        ara::core::Result<void> DTCInformation::SetControlDtcStatusNotifier(
            std::function<void(ControlDtcStatusType)> notifier)
        {
            // In contrast with the ARA standard, no evaluation is perfomed to validate the notifier pointer.
            mControlDtcStatusNotifier = notifier;
            ara::core::Result<void> _result;

            return _result;
        }

        ara::core::Result<void> DTCInformation::EnableControlDtc()
        {
            ara::core::Result<void> _result;

            if (mControlDtcStatus != ControlDtcStatusType::kDTCSettingOn)
            {
                mControlDtcStatus = ControlDtcStatusType::kDTCSettingOn;

                if (mControlDtcStatusNotifier)
                {
                    mControlDtcStatusNotifier(mControlDtcStatus);
                }
            }

            return _result;
        }
    }
}