#include "./security_access.h"

namespace ara
{
    namespace diag
    {
        const uint8_t SecurityAccess::cSid;
        const uint16_t SecurityAccess::cInitialSeed;

        SecurityAccess::SecurityAccess(
            const core::InstanceSpecifier &specifier,
            ReentrancyType reentrancyType) noexcept : routing::RoutableUdsService(specifier, cSid),
                                                      mReentrancy{reentrancyType},
                                                      mSeed{cInitialSeed}
        {
        }

        std::future<OperationOutput> SecurityAccess::HandleMessage(
            const std::vector<uint8_t> &requestData,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            std::promise<OperationOutput> _resultPromise;
            uint8_t _subFunction{requestData.at(cSubFunctionIndex)};

            bool _isOdd{(_subFunction % 2) == 1};

            if (_isOdd)
            {
                OperationOutput _response;
                handleRequestSeed(
                    _response, requestData, metaInfo, std::move(cancellationHandler));

                _resultPromise.set_value(_response);
            }
            else
            {
                std::future_errc _exception;
                std::exception_ptr _exceptionPtr{std::make_exception_ptr(_exception)};

                _resultPromise.set_exception(_exceptionPtr);
            }

            std::future<OperationOutput> _result{_resultPromise.get_future()};

            return _result;
        }

        bool SecurityAccess::validate(uint8_t subFunction)
        {
            // Ignore suppressPosRspMsgIndicationBit
            auto _neutralSubFunction{
                static_cast<uint8_t>(subFunction & (~cSuppressPosRspMask))};

            // Check whether or not the sub-function has been reserved for ISO or the supplier
            if ((_neutralSubFunction == cIsoReservedSubFunction) ||
                (_neutralSubFunction >= cIsoReservedLBound && _neutralSubFunction <= cIsoReservedHBound) ||
                (_neutralSubFunction >= cSupplierReservedLBound && _neutralSubFunction <= cSupplierReservedHBound))
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        bool SecurityAccess::tryFetchSeed(uint8_t level, uint16_t &seed) const
        {
            const uint16_t cUnlockedLevelSeed{0};
            auto _securityLevelItr{mSecurityLevels.find(level)};

            if (_securityLevelItr != mSecurityLevels.end())
            {
                SecurityLevel _securityLevel{_securityLevelItr->second};

                // According to ISO-14229-1-2013 if the level has been already unlocked,
                // returned seed should be zero.
                if (_securityLevel.Unlocked)
                {
                    seed = cUnlockedLevelSeed;
                }
                else
                {
                    seed = _securityLevel.Seed;
                }

                return true;
            }
            else
            {
                return false;
            }
        }

        uint16_t SecurityAccess::addLevel(uint8_t level)
        {
            uint16_t _result{mSeed};
            SecurityLevel _securityLevel(_result);
            mSecurityLevels[level] = _securityLevel;

            // If encountering an overflow while incrementing the seed,
            // reset the seed to its initial value rather than zero, otherwise
            // increment the seed for another security level in future (if requested)
            if (mSeed == std::numeric_limits<uint16_t>().max())
            {
                mSeed = cInitialSeed;
            }
            else
            {
                ++mSeed;
            }

            return _result;
        }

        void SecurityAccess::handleRequestSeed(
            OperationOutput &response,
            const std::vector<uint8_t> &requestData,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            uint8_t _subFunction{requestData.at(cSubFunctionIndex)};
            bool _validSubFunction{validate(_subFunction)};

            if (_validSubFunction)
            {
                auto _dataRecordBeginItr{requestData.cbegin() + cDataRecordOffset};
                auto _dataRecordEndItr{requestData.cend()};
                std::vector<uint8_t> _dataRecord(_dataRecordBeginItr, _dataRecordEndItr);

                auto _seed{
                    GetSeed(
                        _subFunction, _dataRecord, metaInfo, std::move(cancellationHandler))};

                auto _suppressPositiveResponse{
                    static_cast<bool>(_subFunction & cSuppressPosRspMask)};

                if (!_suppressPositiveResponse)
                {
                    response.responseData = _seed.get();
                    auto _sidItr{response.responseData.begin() + cSidIndex};
                    response.responseData.insert(_sidItr, cSid);
                }
            }
            else
            {
                response.responseData =
                    {cNegativeResponseCodeSid,
                     cSid,
                     cSubFunctionNotSupportedNrc};
            }
        }

        std::future<std::vector<uint8_t>> SecurityAccess::GetSeed(
            uint8_t subFunction,
            std::vector<uint8_t> securityAccessDataRecord,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            uint16_t _seed;

            bool _succeed{tryFetchSeed(subFunction, _seed)};

            if (!_succeed)
            {
                _seed = addLevel(subFunction);
            }

            auto _msb{static_cast<uint8_t>(_seed >> 8)};
            auto _lsb{static_cast<uint8_t>(_seed)};
            std::vector<uint8_t> _seedByteArray{_msb, _lsb};

            std::promise<std::vector<uint8_t>> _resultPromise;
            _resultPromise.set_value(_seedByteArray);
            std::future<std::vector<uint8_t>> _result{_resultPromise.get_future()};

            return _result;
        }
    }
}