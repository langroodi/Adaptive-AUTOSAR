#include <utility>
#include <gtest/gtest.h>
#include <doiplib/doip_controller.h>

using namespace DoipLib;

namespace ara
{
    namespace diag
    {
        TEST(DoipLibTest, TryHandlingEmptyRequest)
        {
            ControllerConfig _config;
            _config.protocolVersion = 0x02;
            _config.doipMaxRequestBytes = 64;
            _config.doIPInitialVehicleAnnouncementTime = std::chrono::seconds(0);
            _config.doIPVehicleAnnouncementCount = 3;
            _config.doIPVehicleAnnouncementInterval = std::chrono::seconds(1);

            DoipController _controller(std::move(_config));

            std::vector<uint8_t> _request;
            std::vector<uint8_t> _response;

            EXPECT_TRUE(_controller.TryHandle(_request, _response));
        }
    }
}