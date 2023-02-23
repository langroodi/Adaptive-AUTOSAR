#include <stdexcept>
#include <gtest/gtest.h>
#include <doiplib/announcement_timer.h>

namespace DoipLib
{
    TEST(AnnouncementTimerTest, Constructor)
    {
        const std::chrono::seconds cDelay{1};
        const std::chrono::seconds cInterval{1};
        const uint8_t cCount{0};

        EXPECT_THROW(AnnouncementTimer _timer(cDelay, cInterval, cCount), std::out_of_range);
    }

    TEST(AnnouncementTimerTest, StartMethod)
    {
        const std::chrono::seconds cDelay{0};
        const std::chrono::seconds cInterval{1};
        const uint8_t cExpectedResult{2};

        AnnouncementTimer _timer(cDelay, cInterval, cExpectedResult);
        uint8_t _actualResult{0};

        _timer.Start([&_actualResult]() noexcept { ++_actualResult; });
        
        const int64_t cNyquistGain{2};
        const auto cWaitingTime{
            static_cast<int64_t>(
                (cDelay.count() + cExpectedResult * cInterval.count()) * cNyquistGain)};
        const std::chrono::seconds cWaitingSeconds{cWaitingTime};

        std::this_thread::sleep_for(cWaitingSeconds);
        EXPECT_EQ(cExpectedResult, _actualResult);
    }
}