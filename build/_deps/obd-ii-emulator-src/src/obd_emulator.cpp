#include <stdexcept>
#include "../include/obdemulator/obd_emulator.h"

namespace ObdEmulator
{
    ObdEmulator::ObdEmulator(
        CommunicationLayer *communicationLayer,
        const CanDriver *canDriver,
        std::initializer_list<ObdService *> obdServices) : mCommunicationLayer{communicationLayer},
                                                           mCanDriver{canDriver}
    {
        for (auto service : obdServices)
        {
            uint8_t _mode{service->GetService()};
            mObdServices.emplace(_mode, service);
        }
    }

    bool ObdEmulator::tryParseQuery(
        std::vector<uint8_t> &&query,
        std::vector<uint8_t> &pid,
        uint8_t &queriedService) const
    {
        const size_t cAdditionalDataSizeIndex{0};
        const size_t cServiceIndex{1};
        const size_t cPidIndex{2};

        try
        {
            CanFrame _queryFrame{mCanDriver->Deserialize(query)};

            uint8_t _numberOfAdditionalData{
                _queryFrame.GetData()[cAdditionalDataSizeIndex]};
            auto _additionalDataSize{
                static_cast<size_t>(_numberOfAdditionalData)};

            if (_additionalDataSize > _queryFrame.GetDataLength())
            {
                return false;
            }

            // Extract PID data array
            size_t _pidLength{_additionalDataSize - 1};
            pid = std::vector<uint8_t>(_pidLength);
            std::copy(
                _queryFrame.GetData().cbegin() + cPidIndex,
                _queryFrame.GetData().cbegin() + cPidIndex + _pidLength,
                pid.begin());

            // Search in the registered OBD services
            queriedService = _queryFrame.GetData()[cServiceIndex];
        }
        catch (std::invalid_argument)
        {
            return false;
        }

        return true;
    }

    void ObdEmulator::generateResponse(
        const std::vector<uint8_t> &pid,
        std::vector<uint8_t> &&serviceResponseData,
        uint8_t queriedService,
        std::vector<uint8_t> &response) const
    {
        const uint8_t cResponseServiceOffset{0x40};
        const uint8_t cResponseNotUsedByte{0x55};
        const uint32_t cResponseCanId{0x000007e8};
        const bool cSupportExtendedId{false};
        const bool cSupportRtr{false};

        // Insert the queried PID at the beginning of the response
        serviceResponseData.insert(
            serviceResponseData.begin(),
            pid.begin(),
            pid.end());

        // Insert the response service byte at the beginning
        int _responseServiceInt{queriedService + cResponseServiceOffset};
        auto _responseService{static_cast<uint8_t>(_responseServiceInt)};
        serviceResponseData.insert(
            serviceResponseData.begin(),
            _responseService);

        // Insert the response additional data size at the beginning
        size_t _responseAdditionalDataSize{serviceResponseData.size()};
        auto _numberOfResponseAdditionalData{
            static_cast<uint8_t>(_responseAdditionalDataSize)};
        serviceResponseData.insert(
            serviceResponseData.begin(),
            _numberOfResponseAdditionalData);

        // Fill the rest of the not used data array with a const value based on ISO 15765-2
        for (size_t i = _responseAdditionalDataSize + 1; i < CanFrame::cDataLengthMax; ++i)
        {
            serviceResponseData.push_back(cResponseNotUsedByte);
        }

        CanFrame _responseFrame(
            cResponseCanId, cSupportExtendedId, cSupportRtr, serviceResponseData);

        response = mCanDriver->Serialize(_responseFrame);
    }

    bool ObdEmulator::processQuery(
        std::vector<uint8_t> &&query,
        std::vector<uint8_t> &response) const
    {
        std::vector<uint8_t> _pid;
        uint8_t _queriedService;
        bool _successful = tryParseQuery(std::move(query), _pid, _queriedService);

        if (!_successful)
        {
            return false;
        }

        try
        {
            const ObdService *_obdService{mObdServices.at(_queriedService)};

            std::vector<uint8_t> _serviceResponseData;
            _successful = _obdService->TryGetResponse(_pid, _serviceResponseData);

            if (!_successful)
            {
                return false;
            }

            generateResponse(_pid,
                             std::move(_serviceResponseData),
                             _queriedService,
                             response);
        }
        catch (std::out_of_range)
        {
            return false;
        }

        return true;
    }

    void ObdEmulator::sendResponseAsync(
        std::vector<uint8_t> pid,
        uint8_t queriedService,
        std::vector<uint8_t> &&serviceResponseData)
    {
        std::vector<uint8_t> _response;
        generateResponse(pid,
                         std::move(serviceResponseData),
                         queriedService,
                         _response);

        mCommunicationLayer->TrySendAsync(std::move(_response));
    }

    void ObdEmulator::processQueryAsync(std::vector<uint8_t> &&query)
    {
        std::vector<uint8_t> _pid;
        uint8_t _queriedService;
        const bool cSuccessful{
            tryParseQuery(std::move(query), _pid, _queriedService)};

        if (cSuccessful)
        {
            try
            {
                ObdService *_obdService{mObdServices.at(_queriedService)};
                auto _asyncCallback{
                    std::bind(
                        &ObdEmulator::sendResponseAsync,
                        this, _pid, _queriedService, std::placeholders::_1)};
                _obdService->TryGetResponseAsync(_pid, std::move(_asyncCallback));
            }
            catch (std::out_of_range)
            {
            }
        }
    }

    bool ObdEmulator::TryStart()
    {
        std::vector<uint8_t> _configuration{mCanDriver->GetConfiguration()};
        bool _result{mCommunicationLayer->TryStart(std::move(_configuration))};

        if (_result)
        {
            auto _callback{std::bind(
                &ObdEmulator::processQuery,
                this,
                std::placeholders::_1,
                std::placeholders::_2)};
            mCommunicationLayer->SetCallback(std::move(_callback));
        }

        return _result;
    }

    bool ObdEmulator::TryStartAsync()
    {
        std::vector<uint8_t> _configuration{mCanDriver->GetConfiguration()};
        bool _result{mCommunicationLayer->TryStart(std::move(_configuration))};

        if (_result)
        {
            auto _asyncCallback{std::bind(
                &ObdEmulator::processQueryAsync,
                this,
                std::placeholders::_1)};
            mCommunicationLayer->SetCallback(std::move(_asyncCallback));
        }

        return _result;
    }

    bool ObdEmulator::TryStop()
    {
        bool _result{mCommunicationLayer->TryStop()};

        if (_result)
        {
            mCommunicationLayer->ResetCallback();
        }

        return _result;
    }

    ObdEmulator::~ObdEmulator()
    {
        bool _succeed{TryStop()};

        // Reset the callback anyhow
        if (!_succeed)
        {
            mCommunicationLayer->ResetCallback();
        }
    }
}