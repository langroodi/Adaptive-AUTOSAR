#include <gtest/gtest.h>
#include "../../../src/ara/diag/event.h"

namespace ara
{
    namespace diag
    {
        class EventTest : public testing::Test
        {
        protected:
            const uint8_t cInitialEventState{0x02};

            ara::core::InstanceSpecifier Specifier;
            uint8_t NotifiedEventStatus;

        public:
            EventTest() : Specifier{"Instance0"},
                          NotifiedEventStatus{cInitialEventState}
            {
            }

            void OnEventStatusChanged(EventStatusByte eventStatus)
            {
                NotifiedEventStatus = eventStatus.encodedBits;
            }
        };

        TEST_F(EventTest, Constructor)
        {
            Event _event(Specifier);
            auto _eventStatus{_event.GetEventStatus()};
            uint8_t _actualResult{_eventStatus.Value().encodedBits};

            EXPECT_EQ(cInitialEventState, _actualResult);
        };

        TEST_F(EventTest, EventStatusProperty)
        {
            const uint8_t cExpectedResult{0x82};

            Event _event(Specifier);
            auto _notifier{
                std::bind(
                    &EventTest::OnEventStatusChanged, this, std::placeholders::_1)};

            _event.SetEventStatusChangedNotifier(_notifier);
            _event.SetEventStatusBits({{EventStatusBit::kTestFailed, true}});
            auto _eventStatus{_event.GetEventStatus()};

            EXPECT_EQ(cExpectedResult, NotifiedEventStatus);
        };

        TEST_F(EventTest, LatchedWIRStatusProperty)
        {
            const bool cExpectedResult{true};

            Event _event(Specifier);
            auto _succeed{_event.SetLatchedWIRStatus(cExpectedResult)};
            EXPECT_TRUE(_succeed.HasValue());
            auto _actualResult{_event.GetLatchedWIRStatus()};

            EXPECT_EQ(cExpectedResult, _actualResult.Value());
        }

        TEST_F(EventTest, DtcNumberProperty)
        {
            const uint32_t cExpectedResult{1};

            Event _event(Specifier);
            _event.SetDTCNumber(cExpectedResult);
            auto _actualResult{_event.GetDTCNumber(DTCFormatType::kDTCFormatUDS)};

            EXPECT_EQ(cExpectedResult, _actualResult.Value());
        }

        TEST_F(EventTest, FdcProperty)
        {
            const uint32_t cExpectedResult{1};

            Event _event(Specifier);
            _event.SetFaultDetectionCounter(cExpectedResult);
            auto _actualResult{_event.GetFaultDetectionCounter()};

            EXPECT_EQ(cExpectedResult, _actualResult.Value());
        }

        TEST_F(EventTest, DebouncingStatusProperty)
        {
            const int8_t cFdcMin{-128};
            const int8_t cFdcNeg{-1};
            const int8_t cFdcZero{0};
            const int8_t cFdcPos{1};
            const int8_t cFdcMax{127};

            Event _event(Specifier);

            _event.SetFaultDetectionCounter(cFdcMin);
            EXPECT_EQ(DebouncingState::kFinallyHealed, _event.GetDebouncingStatus());

            _event.SetFaultDetectionCounter(cFdcNeg);
            EXPECT_EQ(DebouncingState::kTemporarilyHealed, _event.GetDebouncingStatus());

            _event.SetFaultDetectionCounter(cFdcZero);
            EXPECT_EQ(DebouncingState::kNeutral, _event.GetDebouncingStatus());

            _event.SetFaultDetectionCounter(cFdcPos);
            EXPECT_EQ(DebouncingState::kTemporarilyDefective, _event.GetDebouncingStatus());

            _event.SetFaultDetectionCounter(cFdcMax);
            EXPECT_EQ(DebouncingState::kFinallyDefective, _event.GetDebouncingStatus());
        }

        TEST_F(EventTest, TestCompleteProperty)
        {
            const int8_t cFdcMin{-128};
            const int8_t cFdcZero{0};
            const int8_t cFdcMax{127};

            Event _event(Specifier);

            _event.SetFaultDetectionCounter(cFdcMin);
            EXPECT_TRUE(_event.GetTestComplete().Value());

            _event.SetFaultDetectionCounter(cFdcZero);
            EXPECT_FALSE(_event.GetTestComplete().Value());

            _event.SetFaultDetectionCounter(cFdcMax);
            EXPECT_TRUE(_event.GetTestComplete().Value());
        }
    }
}