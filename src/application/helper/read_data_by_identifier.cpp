#include "./read_data_by_identifier.h"
#include <iostream>

namespace application
{
    namespace helper
    {
        const uint8_t ReadDataByIdentifier::cSid;
        const uint16_t ReadDataByIdentifier::cAverageSpeedDid;
        const uint16_t ReadDataByIdentifier::cFuelAmountDid;
        const uint16_t ReadDataByIdentifier::cExternalTemperatureDid;
        const uint16_t ReadDataByIdentifier::cAverageFuelConsumptionDid;
        const uint16_t ReadDataByIdentifier::cEngineCoolantTemperatureDid;
        const uint16_t ReadDataByIdentifier::cOdometerValueDid;

        ReadDataByIdentifier::ReadDataByIdentifier(
            const ara::core::InstanceSpecifier &specifier,
            CurlWrapper *curl,
            std::string resourcesUrl) : ara::diag::routing::RoutableUdsService(specifier, cSid),
                                        mCurl{curl},
                                        cResourcesUrl{resourcesUrl}
        {
        }

        uint16_t ReadDataByIdentifier::getDid(const std::vector<uint8_t> &requestData)
        {
            const size_t cDidMsbIndex{0};
            const size_t cDidLsbIndex{1};

            auto _result{static_cast<uint16_t>(requestData.at(cDidMsbIndex) << 8)};
            _result |= requestData.at(cDidLsbIndex);

            return _result;
        }

        void ReadDataByIdentifier::setDid(
            uint16_t did,
            ara::diag::OperationOutput &response)
        {
            const auto cDidMsb{static_cast<uint8_t>(did >> 8)};
            response.responseData.push_back(cDidMsb);

            const uint16_t cMask{0x00ff};
            const auto cDidLsb{static_cast<uint8_t>(did & cMask)};
            response.responseData.push_back(cDidLsb);
        }

        bool ReadDataByIdentifier::tryGetResourceValue(
            std::string resourceKey, Json::Value &jsonValue)
        {
            const std::string cValueKey{"value"};
            const std::string cRequestUrl{cResourcesUrl + "/" + resourceKey};

            std::string _restfulResponse;
            bool _result{mCurl->TryExecute(cRequestUrl, &_restfulResponse)};

            if (_result)
            {
                std::cout << _restfulResponse << std::endl;
                _result =
                    mJsonReader.parse(
                        _restfulResponse, jsonValue, cCollectJsonComments);

                if (_result)
                {
                    jsonValue = jsonValue[resourceKey][cValueKey];
                }
            }

            return _result;
        }

        void ReadDataByIdentifier::getAverageSpeed(
            ara::diag::OperationOutput &response)
        {
            const std::string cResourceKey{"averageSpeed"};
            Json::Value _jsonResponse;

            bool _successful{tryGetResourceValue(cResourceKey, _jsonResponse)};
            if (_successful)
            {
                setDid(cAverageSpeedDid, response);
                const std::string cAverageSpeedStr{_jsonResponse.asString()};
                const auto cAverageSpeed{
                    static_cast<uint8_t>(std::stoul(cAverageSpeedStr))};
                response.responseData.push_back(cAverageSpeed);
            }
            else
            {
                GenerateNegativeResponse(response, cConditionsNotCorrectNrc);
            }
        }

        void ReadDataByIdentifier::getFuelAmount(
            ara::diag::OperationOutput &response)
        {
            const double cConversionGain{2.55};
            const std::string cResourceKey{"fuelAmount"};
            Json::Value _jsonResponse;

            bool _successful{tryGetResourceValue(cResourceKey, _jsonResponse)};
            if (_successful)
            {
                setDid(cFuelAmountDid, response);
                const std::string cFuelAmountStr{_jsonResponse.asString()};
                const unsigned long cFuelAmountUL{std::stoul(cFuelAmountStr)};
                const auto cFuelAmount{
                    static_cast<uint8_t>(cConversionGain * cFuelAmountUL)};
                response.responseData.push_back(cFuelAmount);
            }
            else
            {
                GenerateNegativeResponse(response, cConditionsNotCorrectNrc);
            }
        }

        void ReadDataByIdentifier::getExternalTemperature(
            ara::diag::OperationOutput &response)
        {
            const uint8_t cCompensationValue{40};
            const std::string cResourceKey{"externalTemp"};
            Json::Value _jsonResponse;

            bool _successful{tryGetResourceValue(cResourceKey, _jsonResponse)};
            if (_successful)
            {
                setDid(cExternalTemperatureDid, response);
                const std::string cExternalTemperatureStr{
                    _jsonResponse.asString()};
                const unsigned long cExternalTemperatureUL{
                    std::stoul(cExternalTemperatureStr)};
                const auto cExternalTemperature{
                    static_cast<uint8_t>(
                        cExternalTemperatureUL + cCompensationValue)};
                response.responseData.push_back(cExternalTemperature);
            }
            else
            {
                GenerateNegativeResponse(response, cConditionsNotCorrectNrc);
            }
        }

        void ReadDataByIdentifier::getAverageFuelConsumption(
            ara::diag::OperationOutput &response)
        {
            const uint16_t cConversionGain{20};
            const uint16_t cConversionBase{256};
            const std::string cResourceKey{"averageFuelConsumption"};
            Json::Value _jsonResponse;

            bool _successful{tryGetResourceValue(cResourceKey, _jsonResponse)};
            if (_successful)
            {
                setDid(cAverageFuelConsumptionDid, response);
                const std::string cAverageFuelConsumptionStr{
                    _jsonResponse.asString()};
                const double cAverageFuelConsumption{
                    std::stod(cAverageFuelConsumptionStr)};
                const auto cAverageFuelConsumptionInt{
                    static_cast<uint16_t>(
                        cConversionGain * cAverageFuelConsumption)};

                const auto cAverageFuelConsumptionMsb{
                    static_cast<uint8_t>(
                        cAverageFuelConsumptionInt / cConversionBase)};
                response.responseData.push_back(cAverageFuelConsumptionMsb);

                const auto cAverageFuelConsumptionLsb{
                    static_cast<uint8_t>(
                        cAverageFuelConsumptionInt % cConversionBase)};
                response.responseData.push_back(cAverageFuelConsumptionLsb);
            }
            else
            {
                GenerateNegativeResponse(response, cConditionsNotCorrectNrc);
            }
        }

        void ReadDataByIdentifier::getEngineCoolantTemperature(
            ara::diag::OperationOutput &response)
        {
            const uint8_t cCompensationValue{40};
            const std::string cResourceKey{"engineCoolantTemp"};
            Json::Value _jsonResponse;

            bool _successful{tryGetResourceValue(cResourceKey, _jsonResponse)};
            if (_successful)
            {
                setDid(cEngineCoolantTemperatureDid, response);
                const std::string cEngineCoolantTemperatureStr{
                    _jsonResponse.asString()};
                const unsigned long cEngineCoolantTemperatureUL{
                    std::stoul(cEngineCoolantTemperatureStr)};
                const auto cEngineCoolantTemperature{
                    static_cast<uint8_t>(
                        cEngineCoolantTemperatureUL + cCompensationValue)};
                response.responseData.push_back(cEngineCoolantTemperature);
            }
            else
            {
                GenerateNegativeResponse(response, cConditionsNotCorrectNrc);
            }
        }

        void ReadDataByIdentifier::getOdometerValue(
            ara::diag::OperationOutput &response)
        {
            constexpr size_t cCount{3};
            const uint32_t cConversionGain{10};
            const uint32_t cConversionBases[cCount]{16777216, 65536, 256};
            const std::string cResourceKey{"odometer"};
            Json::Value _jsonResponse;

            bool _successful{tryGetResourceValue(cResourceKey, _jsonResponse)};
            if (_successful)
            {
                setDid(cOdometerValueDid, response);
                const std::string cOdometerValueStr{_jsonResponse.asString()};
                const double cOdometerValue{std::stod(cOdometerValueStr)};
                auto _odometerValueInt{
                    static_cast<uint32_t>(cConversionGain * cOdometerValue)};

                for (size_t i = 0; i < cCount; ++i)
                {
                    const auto cOdometerValueMsb{
                        static_cast<uint8_t>(
                            _odometerValueInt / cConversionBases[i])};

                    response.responseData.push_back(cOdometerValueMsb);
                    _odometerValueInt %=  cConversionBases[i];
                }

                const auto cOdometerValueLsb{
                    static_cast<uint8_t>(_odometerValueInt)};
                response.responseData.push_back(cOdometerValueLsb);
            }
            else
            {
                GenerateNegativeResponse(response, cConditionsNotCorrectNrc);
            }
        }

        std::future<ara::diag::OperationOutput> ReadDataByIdentifier::HandleMessage(
            const std::vector<uint8_t> &requestData,
            ara::diag::MetaInfo &metaInfo,
            ara::diag::CancellationHandler &&cancellationHandler)
        {
            const uint16_t cDid{getDid(requestData)};
            ara::diag::OperationOutput _response;
            std::promise<ara::diag::OperationOutput> _resultPromise;

            switch (cDid)
            {
            case cAverageSpeedDid:
                getAverageSpeed(_response);
                break;
            case cFuelAmountDid:
                getFuelAmount(_response);
                break;
            case cExternalTemperatureDid:
                getExternalTemperature(_response);
                break;
            case cAverageFuelConsumptionDid:
                getAverageFuelConsumption(_response);
                break;
            case cEngineCoolantTemperatureDid:
                getEngineCoolantTemperature(_response);
                break;
            case cOdometerValueDid:
                getOdometerValue(_response);
                break;
            default:
                GenerateNegativeResponse(_response, cRequestOutOfRangeNrc);
            }

            _resultPromise.set_value(_response);
            std::future<ara::diag::OperationOutput> _result{
                _resultPromise.get_future()};

            return _result;
        }
    }
}