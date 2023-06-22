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
                                                      mSeed{cInitialSeed},
                                                      mFailedUnlockAttempt{0}
        {
        }

        bool SecurityAccess::hasProblem(
            const std::vector<uint8_t> &requestData, uint8_t &nrc) const
        {
            if (requestData.size() > cSubFunctionIndex)
            {
                uint8_t _subFunction{requestData.at(cSubFunctionIndex)};

                // Ignore suppressPosRspMsgIndicationBit
                auto _neutralSubFunction{
                    static_cast<uint8_t>(_subFunction & (~cSuppressPosRspMask))};

                if ((_neutralSubFunction == cIsoReservedSubFunction) ||
                    (_neutralSubFunction >= cIsoReservedLBound && _neutralSubFunction <= cIsoReservedHBound) ||
                    (_neutralSubFunction >= cSupplierReservedLBound && _neutralSubFunction <= cSupplierReservedHBound))
                {
                    // The sub-function has been reserved for ISO or the supplier.
                    nrc = cSubFunctionNotSupported;
                    return true;
                }
                else if (mDelayTimer.IsActive())
                {
                    // The delay timer is active due to exceeding the number of unlock attempts.
                    nrc = cExceededNumberOfAttempts;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                // The request payload length is incorrect.
                nrc = cIncorrectMessageLength;
                return true;
            }
        }

        std::future<OperationOutput> SecurityAccess::HandleMessage(
            const std::vector<uint8_t> &requestData,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            uint8_t _nrc;
            OperationOutput _response;
            std::promise<OperationOutput> _resultPromise;

            bool _validRequest{!hasProblem(requestData, _nrc)};

            if (_validRequest)
            {
                uint8_t _subFunction{requestData.at(cSubFunctionIndex)};
                auto _parametersBeginItr{requestData.cbegin() + cDataRecordOffset};
                auto _parametersEndItr{requestData.cend()};
                std::vector<uint8_t> _parameters(_parametersBeginItr, _parametersEndItr);

                auto _suppressPositiveResponse{
                    static_cast<bool>(_subFunction & cSuppressPosRspMask)};

                bool _isOdd{(_subFunction % 2) == 1};

                if (_isOdd)
                {
                    handleRequestSeed(
                        _response,
                        _subFunction,
                        _parameters,
                        metaInfo,
                        std::move(cancellationHandler),
                        _suppressPositiveResponse);
                }
                else
                {
                    handleSendKey(
                        _response,
                        _subFunction,
                        _parameters,
                        metaInfo,
                        std::move(cancellationHandler),
                        _suppressPositiveResponse);
                }
            }
            else if (cancellationHandler.IsCanceled())
            {
                GenerateNegativeResponse(_response, cGeneralReject);
            }
            else
            {
                GenerateNegativeResponse(_response, _nrc);
            }

            _resultPromise.set_value(_response);
            std::future<OperationOutput> _result{_resultPromise.get_future()};

            return _result;
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
            uint8_t subFunction,
            const std::vector<uint8_t> &securityAccessDataRecord,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler,
            bool suppressPositiveResponse)
        {
            auto _seed{
                GetSeed(
                    subFunction,
                    securityAccessDataRecord,
                    metaInfo,
                    std::move(cancellationHandler))};

            if (!suppressPositiveResponse)
            {
                response.responseData = _seed.get();
                auto _sidItr{response.responseData.begin() + cSidIndex};
                response.responseData.insert(_sidItr, cSid);
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

        void SecurityAccess::handlePassedAttempt(
            std::map<uint8_t, SecurityLevel>::iterator securityLevelItr)
        {
            // Lock all the security levels expect the recentlly unlocked one
            for (auto _securityLevel : mSecurityLevels)
            {
                _securityLevel.second.Unlocked = false;
            }

            securityLevelItr->second.Unlocked = true;
        }

        void SecurityAccess::handleFailedAttempt(MetaInfo &metaInfo)
        {
            // If the delay timer has been already activated, no need to handle the failed attempt
            if (mDelayTimer.IsActive())
            {
                return;
            }

            uint8_t _attemptThreshold;
            std::chrono::seconds _exceededAttemptDelay;

            bool _succeed{
                TryExtractValue(
                    metaInfo, cAttemptThresholdKey, _attemptThreshold)};

            if (_succeed)
            {
                _succeed =
                    TryExtractValue(
                        metaInfo, cExceededAttemptDelayKey, _exceededAttemptDelay);
            }

            if (_succeed)
            {
                // Increment the failed attempt
                ++mFailedUnlockAttempt;

                if (mFailedUnlockAttempt > _attemptThreshold)
                {
                    mDelayTimer.Start(_exceededAttemptDelay);
                    // Reset the number of failed attempts
                    mFailedUnlockAttempt = 0;
                }
            }
        }

        void SecurityAccess::handleSendKey(
            OperationOutput &response,
            uint8_t subFunction,
            const std::vector<uint8_t> &key,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler,
            bool suppressPositiveResponse)
        {
            if (key.size() == cKeyLength)
            {
                // Corresponding request seed sub-function is always one before the send key sub-function.
                auto _requestSeedSubFunction{static_cast<uint8_t>(subFunction - 1)};
                auto _securityLevelItr{mSecurityLevels.find(_requestSeedSubFunction)};

                if (_securityLevelItr != mSecurityLevels.end())
                {
                    std::future<KeyCompareResultType> _keyCompareResultFuture{
                        CompareKey(
                            _requestSeedSubFunction,
                            key,
                            metaInfo,
                            std::move(cancellationHandler))};

                    KeyCompareResultType _keyCompareResult{_keyCompareResultFuture.get()};

                    if (_keyCompareResult == KeyCompareResultType::kKeyValid)
                    {
                        handlePassedAttempt(_securityLevelItr);
                        if (!suppressPositiveResponse)
                        {
                            auto _sid{static_cast<uint8_t>(cSid + cPositiveResponseSidIncrement)};
                            response.responseData = {_sid, subFunction};
                        }
                    }
                    else
                    {
                        handleFailedAttempt(metaInfo);
                        // Sent key is invalid.
                        GenerateNegativeResponse(response, mDelayTimer.IsActive() ? cExceededNumberOfAttempts : cInvalidKey);
                    }
                }
                else
                {
                    // Send key request is received before the request seed.
                    GenerateNegativeResponse(response, cRequestSequenceError);
                }
            }
            else
            {
                // Key length is incorrect.
                GenerateNegativeResponse(response, cIncorrectMessageLength);
            }
        }

        std::future<KeyCompareResultType> SecurityAccess::CompareKey(
            uint8_t subFunction,
            std::vector<uint8_t> key,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            std::promise<KeyCompareResultType> _resultPromise;

            uint16_t _encryptor;
            bool _succeed{TryExtractValue(metaInfo, cEncryptorKey, _encryptor)};

            if (_succeed)
            {
                SecurityLevel _securityLevel{mSecurityLevels.at(subFunction)};
                uint16_t _seed{_securityLevel.Seed};

                // Compute XOR expected cipher
                auto _expectedCipher{static_cast<uint16_t>(_seed ^ _encryptor)};
                // Recover the received cipher from the byte array
                auto _receivedCipher{static_cast<uint16_t>(key.at(0)) << 8};
                _receivedCipher |= key.at(1);

                _resultPromise.set_value(
                    (_expectedCipher == _receivedCipher) ? KeyCompareResultType::kKeyValid : KeyCompareResultType::kKeyInvalid);
            }
            else
            {
                _resultPromise.set_value(KeyCompareResultType::kKeyInvalid);
            }

            std::future<KeyCompareResultType> _result{_resultPromise.get_future()};

            return _result;
        }
    }
}