#include "./dtc_information.h"
#include "./diag_error_domain.h"

namespace ara
{
    namespace diag
    {
        DTCInformation::DTCInformation(const core::InstanceSpecifier &specifier) : mSpecifier{specifier},
                                                                                   mControlDtcStatus{ControlDtcStatusType::kDTCSettingOff}
        {
        }

        core::Result<UdsDtcStatusByteType> DTCInformation::GetCurrentStatus(uint32_t dtc)
        {
            try
            {
                UdsDtcStatusByteType _status{mStatuses.at(dtc)};
                core::Result<UdsDtcStatusByteType> _result{_status};

                return _result;
            }
            catch (std::out_of_range)
            {
                core::ErrorDomain *_errorDomain{DiagErrorDomain::GetDiagDomain()};
                auto _diagErrorDomain{static_cast<DiagErrorDomain *>(_errorDomain)};
                core::ErrorCode _errorCode{_diagErrorDomain->MakeErrorCode(DiagErrc::kNoSuchDTC)};
                auto _result{core::Result<UdsDtcStatusByteType>::FromError(_errorCode)};

                return _result;
            }
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

        core::Result<void> DTCInformation::SetDTCStatusChangedNotifier(
            std::function<void(uint32_t, UdsDtcStatusByteType, UdsDtcStatusByteType)> notifier)
        {
            // In contrast with the ARA standard, no evaluation is perfomed to validate the notifier pointer.
            mDtcStatusChangedNotifier = notifier;
            core::Result<void> _result;

            return _result;
        }

        core::Result<uint32_t> DTCInformation::GetNumberOfStoredEntries()
        {
            auto _size{static_cast<uint32_t>(mStatuses.size())};
            core::Result<uint32_t> _result{_size};

            return _result;
        }

        core::Result<void> DTCInformation::SetNumberOfStoredEntriesNotifier(
            std::function<void(uint32_t)> notifier)
        {
            // In contrast with the ARA standard, no evaluation is perfomed to validate the notifier pointer.
            mNumberOfStoredEntriesNotifier = notifier;
            core::Result<void> _result;

            return _result;
        }

        core::Result<void> DTCInformation::Clear(uint32_t dtc)
        {

            auto _iterator{mStatuses.find(dtc)};

            if (_iterator != mStatuses.end())
            {
                mStatuses.erase(_iterator);
                if (mNumberOfStoredEntriesNotifier)
                {
                    mNumberOfStoredEntriesNotifier(mStatuses.size());
                }

                core::Result<void> _result;
                return _result;
            }
            else
            {
                core::ErrorDomain *_errorDomain{DiagErrorDomain::GetDiagDomain()};
                auto _diagErrorDomain{static_cast<DiagErrorDomain *>(_errorDomain)};
                core::ErrorCode _errorCode{_diagErrorDomain->MakeErrorCode(DiagErrc::kWrongDtc)};
                auto _result{core::Result<void>::FromError(_errorCode)};

                return _result;
            }
        }

        core::Result<ControlDtcStatusType> DTCInformation::GetControlDTCStatus()
        {
            core::Result<ControlDtcStatusType> _result{mControlDtcStatus};
            return _result;
        }

        core::Result<void> DTCInformation::SetControlDtcStatusNotifier(
            std::function<void(ControlDtcStatusType)> notifier)
        {
            // In contrast with the ARA standard, no evaluation is perfomed to validate the notifier pointer.
            mControlDtcStatusNotifier = notifier;
            core::Result<void> _result;

            return _result;
        }

        core::Result<void> DTCInformation::EnableControlDtc()
        {
            core::Result<void> _result;

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