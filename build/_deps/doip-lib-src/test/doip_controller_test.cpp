#include <utility>
#include <gtest/gtest.h>
#include <doiplib/doip_controller.h>
#include <doiplib/generic_nack.h>
#include "./dummy_message_handler.h"

namespace DoipLib
{
    class DoipControllerTest : public testing::Test
    {
    protected:
        static const uint16_t cSourceAddress{0x0001};

        const uint32_t cDoipMaxRequestBytes{8};
        const uint8_t cProtocolVersion{2};
        const std::chrono::seconds cDelay{0};
        const std::chrono::seconds cInterval{1};
        uint8_t cCount{3};

        DummyMessageHandler mHandler;

        DoipControllerTest() : mHandler(cSourceAddress)
        {
        }

        void RegisterHandler(DoipController &controller)
        {
            auto _handler{static_cast<MessageHandler *>(&mHandler)};
            controller.Register(PayloadType::AliveCheckRequest, _handler);
        }

        bool TryParseNackCode(const std::vector<uint8_t> &response, GenericNackType &nackCode)
        {
            GenericNack _genericNack;
            bool _result{_genericNack.TryDeserialize(response, nackCode)};

            if (_result)
            {
                nackCode = _genericNack.GetNackCode();
            }

            return _result;
        }
    };

    const uint8_t DummyMessageHandler::cProtocolVersion;
    const uint16_t DoipControllerTest::cSourceAddress;

    TEST_F(DoipControllerTest, ValidRequestHandling)
    {
        const std::vector<uint8_t> cPayload{
            0x02, 0xfd, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00};

        ControllerConfig _config;
        _config.doipMaxRequestBytes = cDoipMaxRequestBytes;
        _config.protocolVersion = cProtocolVersion;
        _config.doIPInitialVehicleAnnouncementTime = cDelay;
        _config.doIPVehicleAnnouncementInterval = cInterval;
        _config.doIPVehicleAnnouncementCount = cCount;

        DoipController _controller(std::move(_config));
        RegisterHandler(_controller);

        std::vector<uint8_t> _response;
        EXPECT_TRUE(_controller.TryHandle(cPayload, _response));

        AliveCheckResponse _aliveCheckResponse;
        GenericNackType _nackCode;
        EXPECT_TRUE(_aliveCheckResponse.TryDeserialize(_response, _nackCode));

        uint16_t _actualResult{_aliveCheckResponse.GetSourceAddress()};
        EXPECT_EQ(cSourceAddress, _actualResult);
    }

    TEST_F(DoipControllerTest, InvalidLargeRequestHandling)
    {
        const GenericNackType cExpectedResult{GenericNackType::ToolLargeMessage};

        const std::vector<uint8_t> cPayload{
            0x02, 0xfd, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0xff};

        ControllerConfig _config;
        _config.doipMaxRequestBytes = cDoipMaxRequestBytes;
        _config.protocolVersion = cProtocolVersion;
        _config.doIPInitialVehicleAnnouncementTime = cDelay;
        _config.doIPVehicleAnnouncementInterval = cInterval;
        _config.doIPVehicleAnnouncementCount = cCount;

        DoipController _controller(std::move(_config));
        RegisterHandler(_controller);

        std::vector<uint8_t> _response;
        EXPECT_TRUE(_controller.TryHandle(cPayload, _response));

        GenericNackType _actualResult;
        EXPECT_TRUE(TryParseNackCode(_response, _actualResult));
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST_F(DoipControllerTest, InvalidTypeRequestHandling)
    {
        const GenericNackType cExpectedResult{GenericNackType::UnsupportedPayloadType};

        const std::vector<uint8_t> cPayload{
            0x02, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};

        ControllerConfig _config;
        _config.doipMaxRequestBytes = cDoipMaxRequestBytes;
        _config.protocolVersion = cProtocolVersion;
        _config.doIPInitialVehicleAnnouncementTime = cDelay;
        _config.doIPVehicleAnnouncementInterval = cInterval;
        _config.doIPVehicleAnnouncementCount = cCount;

        DoipController _controller(std::move(_config));
        RegisterHandler(_controller);

        std::vector<uint8_t> _response;
        EXPECT_TRUE(_controller.TryHandle(cPayload, _response));

        GenericNackType _actualResult;
        EXPECT_TRUE(TryParseNackCode(_response, _actualResult));
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST_F(DoipControllerTest, InvalidLengthRequestHandling)
    {
        const GenericNackType cExpectedResult{GenericNackType::InvalidPayloadLength};

        const std::vector<uint8_t> cPayload{
            0x02, 0xfd, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0xff};

        ControllerConfig _config;
        _config.doipMaxRequestBytes = static_cast<uint32_t>(cPayload.size());
        _config.protocolVersion = cProtocolVersion;
        _config.doIPInitialVehicleAnnouncementTime = cDelay;
        _config.doIPVehicleAnnouncementInterval = cInterval;
        _config.doIPVehicleAnnouncementCount = cCount;

        DoipController _controller(std::move(_config));
        RegisterHandler(_controller);

        std::vector<uint8_t> _response;
        EXPECT_TRUE(_controller.TryHandle(cPayload, _response));

        GenericNackType _actualResult;
        EXPECT_TRUE(TryParseNackCode(_response, _actualResult));
        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST_F(DoipControllerTest, EmptyRequestHandling)
    {
        const GenericNackType cExpectedResult{GenericNackType::InvalidPayloadLength};

        const std::vector<uint8_t> cPayload;

        ControllerConfig _config;
        _config.doipMaxRequestBytes = static_cast<uint32_t>(cPayload.size());
        _config.protocolVersion = cProtocolVersion;
        _config.doIPInitialVehicleAnnouncementTime = cDelay;
        _config.doIPVehicleAnnouncementInterval = cInterval;
        _config.doIPVehicleAnnouncementCount = cCount;

        DoipController _controller(std::move(_config));
        RegisterHandler(_controller);

        std::vector<uint8_t> _response;
        EXPECT_TRUE(_controller.TryHandle(cPayload, _response));

        GenericNackType _actualResult;
        EXPECT_TRUE(TryParseNackCode(_response, _actualResult));
        EXPECT_EQ(cExpectedResult, _actualResult);
    }
}