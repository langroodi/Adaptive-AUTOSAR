#include <gtest/gtest.h>
#include <obdemulator/obd_emulator.h>
#include "./helpers/loopback_communication_layer.h"
#include "./helpers/dummy_obd_service.h"

namespace ObdEmulator
{
    class ObdEmulatorTest : public testing::Test
    {
    protected:
        const CanBusSpeed cSpeed{CanBusSpeed::Speed5kbps};
        const bool cSupportExtended{false};
        const uint32_t cQueryBroadcastId{0x7df};
        const bool cIsExtended{cSupportExtended};
        const bool cIsRtr{false};
        const uint8_t cQuerySize{2};
        const uint8_t cResponseServiceOfsset{0x40};
        const size_t cResponseServiceIndex{1};
        const size_t cResponseDataIndex{3};

        CanDriver mCanDriver;
        Helpers::LoopbackCommunicationLayer mCommunicationLayer;
        Helpers::DummyObdService mObdService;
        ObdEmulator mObdEmulator;

        ObdEmulatorTest() : mCanDriver(cSpeed, cSupportExtended),
                            mCommunicationLayer(&mCanDriver),
                            mObdEmulator(&mCommunicationLayer, &mCanDriver, {&mObdService})
        {
        }
    };

    TEST_F(ObdEmulatorTest, LoopbackScenario)
    {
        EXPECT_TRUE(mObdEmulator.TryStart());

        CanFrame _query(
            cQueryBroadcastId,
            cIsExtended,
            cIsRtr,
            {cQuerySize, Helpers::DummyObdService::cService, Helpers::DummyObdService::cVehicleSpeedPid});

        CanFrame _response{mCommunicationLayer.Send(_query)};

        uint8_t _actualService{_response.GetData().at(cResponseServiceIndex)};
        int _expectedServiceInt{Helpers::DummyObdService::cService + cResponseServiceOfsset};
        auto _expectedService{static_cast<uint8_t>(_expectedServiceInt)};
        EXPECT_EQ(_actualService, _expectedService);

        uint8_t _actualSpeed{_response.GetData().at(cResponseDataIndex)};
        EXPECT_EQ(_actualSpeed, Helpers::DummyObdService::cVehicleSpeed);

        EXPECT_TRUE(mObdEmulator.TryStop());
    }

    TEST_F(ObdEmulatorTest, AsyncLoopbackScenario)
    {
        EXPECT_TRUE(mObdEmulator.TryStartAsync());

        CanFrame _query(
            cQueryBroadcastId,
            cIsExtended,
            cIsRtr,
            {cQuerySize, Helpers::DummyObdService::cService, Helpers::DummyObdService::cVehicleSpeedPid});

        mCommunicationLayer.SendAsync(_query);

        const CanFrame *_response;
        EXPECT_TRUE(mCommunicationLayer.TryGetLastReceivedCanFrame(_response));

        uint8_t _actualService{_response->GetData().at(cResponseServiceIndex)};
        int _expectedServiceInt{Helpers::DummyObdService::cService + cResponseServiceOfsset};
        auto _expectedService{static_cast<uint8_t>(_expectedServiceInt)};
        EXPECT_EQ(_actualService, _expectedService);

        uint8_t _actualSpeed{_response->GetData().at(cResponseDataIndex)};
        EXPECT_EQ(_actualSpeed, Helpers::DummyObdService::cVehicleSpeed);

        EXPECT_TRUE(mObdEmulator.TryStop());
    }
}