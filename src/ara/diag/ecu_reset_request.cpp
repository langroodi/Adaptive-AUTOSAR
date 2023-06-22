#include <stdexcept>
#include "./ecu_reset_request.h"

namespace ara
{
    namespace diag
    {
        const uint8_t EcuResetRequest::cSid;

        EcuResetRequest::EcuResetRequest(
            const core::InstanceSpecifier &specifier) : routing::RoutableUdsService(specifier, cSid),
                                                        mRapidShutdownEnabled{false}
        {
        }

        bool EcuResetRequest::TryHandleSubFunction(
            uint8_t subFunction,
            const MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler,
            bool &powerDownTimeRequired)
        {
            const uint8_t cHardResetSubFunction{0x01};
            const uint8_t cKeyOffOnResetSubFunction{0x02};
            const uint8_t cSoftResetSubFunction{0x03};
            const uint8_t cEnableRapidShutdownSubFunction{0x04};
            const uint8_t cDisableRapidShutdownSubFunction{0x05};

            core::Optional<uint8_t> _id{0x00};
            std::future<void> _succeedFuture;

            switch (subFunction)
            {
            case cHardResetSubFunction:
                _succeedFuture =
                    RequestReset(
                        ResetRequestType::kHardReset,
                        _id,
                        metaInfo,
                        std::move(cancellationHandler));
                break;

            case cKeyOffOnResetSubFunction:
                _succeedFuture =
                    RequestReset(
                        ResetRequestType::kKeyOffOnReset,
                        _id,
                        metaInfo,
                        std::move(cancellationHandler));
                break;

            case cSoftResetSubFunction:
                _succeedFuture =
                    RequestReset(
                        ResetRequestType::kSoftReset,
                        _id,
                        metaInfo,
                        std::move(cancellationHandler));
                break;

            case cEnableRapidShutdownSubFunction:
                _succeedFuture =
                    EnableRapidShutdown(
                        true,
                        metaInfo,
                        std::move(cancellationHandler));
                break;

            case cDisableRapidShutdownSubFunction:
                _succeedFuture =
                    EnableRapidShutdown(
                        false,
                        metaInfo,
                        std::move(cancellationHandler));
                break;
            }

            bool _result{_succeedFuture.valid()};
            if (_result)
            {
                powerDownTimeRequired =
                    (subFunction == cEnableRapidShutdownSubFunction);
            }

            return _result;
        }

        std::future<OperationOutput> EcuResetRequest::HandleMessage(
            const std::vector<uint8_t> &requestData,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            const size_t cSubFunctionIndex{1};
            const std::string cPowerDownTimeKey{"PowerDownTime"};
            const auto cResponseSid{
                static_cast<uint8_t>(cSid + cPositiveResponseSidIncrement)};

            OperationOutput _response;
            std::promise<OperationOutput> _resultPromise;
            std::future<OperationOutput> _result{_resultPromise.get_future()};

            if (requestData.size() > cSubFunctionIndex)
            {
                uint8_t _subFunction{requestData.at(cSubFunctionIndex)};
                bool _powerDownTimeRequired;
                bool _succeed{
                    TryHandleSubFunction(
                        _subFunction, metaInfo, std::move(cancellationHandler),
                        _powerDownTimeRequired)};

                uint8_t _powerDownTime;
                if (_succeed && _powerDownTimeRequired)
                {
                    _succeed =
                        TryExtractValue<uint8_t>(
                            metaInfo, cPowerDownTimeKey, _powerDownTime);
                }

                if (_succeed)
                {
                    _response.responseData = {cResponseSid, _subFunction};

                    // Insert the power down-time at the of the response (if required)
                    if (_powerDownTimeRequired)
                    {
                        _response.responseData.insert(
                            _response.responseData.end(), _powerDownTime);
                    }
                }
                else
                {
                    GenerateNegativeResponse(_response, cSubFunctionNotSupported);
                }
            }
            else
            {
                GenerateNegativeResponse(_response, cIncorrectMessageLength);
            }

            _resultPromise.set_value(_response);

            return _result;
        }

        std::future<void> EcuResetRequest::RequestReset(
            ResetRequestType resetType,
            core::Optional<uint8_t> id,
            const MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            std::promise<void> _resultPromise;
            std::future<void> _result;

            // Set the requested reset type if it has not been requested yet
            if (!mResetTypeFuture.valid())
            {
                mResetTypeFuture = mResetTypePromise.get_future();
                mResetTypePromise.set_value(resetType);

                _result = _resultPromise.get_future();
                _resultPromise.set_value();
            }

            return _result;
        }

        void EcuResetRequest::ExecuteReset(const MetaInfo &metaInfo)
        {
            if (mResetTypeFuture.valid())
            {
                throw std::logic_error("ECU reset is not supported.");
            }
            else
            {
                throw std::logic_error("No reset type has been requested yet.");
            }
        }

        std::future<void> EcuResetRequest::EnableRapidShutdown(
            bool enable,
            const MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            mRapidShutdownEnabled = enable;
            std::promise<void> _resultPromise;
            std::future<void> _result{_resultPromise.get_future()};
            _resultPromise.set_value();

            return _result;
        }
    }
}