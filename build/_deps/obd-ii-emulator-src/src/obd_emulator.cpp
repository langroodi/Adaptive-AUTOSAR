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

    bool ObdEmulator::processQuery(
        std::vector<uint8_t> &&query,
        std::vector<uint8_t> &response)
    {
        const size_t cAdditionalDataSizeIndex{0};
        const size_t cServiceIndex{1};
        const size_t cPidIndex{2};
        const uint8_t cResponseServiceOffset{0x40};
        const uint8_t cResponseNotUsedByte{0x55};
        const uint32_t cResponseCanId{0x000007e8};
        const bool cSupportExtendedId{false};
        const bool cSupportRtr{false};

        bool _result;
        try
        {
            CanFrame _queryFrame{mCanDriver->Deserialize(query)};

            uint8_t _numberOfAdditionalData{
                _queryFrame.GetData()[cAdditionalDataSizeIndex]};
            auto _additionalDataSize{
                static_cast<size_t>(_numberOfAdditionalData)};

            if (_additionalDataSize > _queryFrame.GetDataLength())
            {
                throw std::out_of_range("The data segment is corrupted.");
            }

            // Search in the registered OBD services
            uint8_t _queriedService{_queryFrame.GetData()[cServiceIndex]};
            ObdService *_obdService{mObdServices.at(_queriedService)};

            // Extract PID data array
            size_t _pidLength{_additionalDataSize - 1};
            std::vector<uint8_t> _pid(_pidLength);
            std::copy(
                _queryFrame.GetData().cbegin() + cPidIndex,
                _queryFrame.GetData().cbegin() + cPidIndex + _pidLength,
                _pid.begin());

            std::vector<uint8_t> _serviceResponseData;
            _result = _obdService->TryGetResponse(_pid, _serviceResponseData);

            // Insert the queried PID at the beginning of the response
            _serviceResponseData.insert(
                _serviceResponseData.begin(),
                _pid.begin(),
                _pid.end());

            // Insert the response service byte at the beginning
            int _responseServiceInt{_queriedService + cResponseServiceOffset};
            auto _responseService{static_cast<uint8_t>(_responseServiceInt)};
            _serviceResponseData.insert(
                _serviceResponseData.begin(),
                _responseService);

            // Insert the response additional data size at the beginning
            size_t _responseAdditionalDataSize{_serviceResponseData.size()};
            auto _numberOfResponseAdditionalData{
                static_cast<uint8_t>(_responseAdditionalDataSize)};
            _serviceResponseData.insert(
                _serviceResponseData.begin(),
                _numberOfResponseAdditionalData);

            // Fill the rest of the not used data array with a const value based on ISO 15765-2
            for (size_t i = _responseAdditionalDataSize + 1; i < CanFrame::cDataLengthMax; ++i)
            {
                _serviceResponseData.push_back(cResponseNotUsedByte);
            }

            CanFrame _responseFrame(
                cResponseCanId, cSupportExtendedId, cSupportRtr, _serviceResponseData);

            response = mCanDriver->Serialize(_responseFrame);
        }
        catch (const std::invalid_argument &ex)
        {
            _result = false;
        }
        catch (const std::out_of_range &ex)
        {
            _result = false;
        }

        return _result;
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
            mCommunicationLayer->SetCallback(_callback);
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