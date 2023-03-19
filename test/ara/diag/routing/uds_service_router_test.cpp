#include <gtest/gtest.h>
#include "../../../../src/ara/diag/routing/uds_service_router.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            TEST(UdsServiceRouterTest, ValidRouteMethod)
            {
                const uint8_t cExpectedResult{0x21};
                const size_t cSidIndex{1};

                UdsServiceRouter _router;
                std::vector<uint8_t> _requestData{cExpectedResult};
                MetaInfo _metaInfo(Context::kDoIP);
                CancellationHandler _cancellationHandler(false);

                std::future<OperationOutput> _response{
                    _router.Route(
                        _requestData,
                        _metaInfo,
                        std::move(_cancellationHandler))};

                EXPECT_TRUE(_response.valid());
                OperationOutput _operationOutput{_response.get()};
                uint8_t _actualResult{_operationOutput.responseData.at(cSidIndex)};

                EXPECT_EQ(cExpectedResult, _actualResult);
            }

            TEST(UdsServiceRouterTest, InvalidRouteMethod)
            {
                UdsServiceRouter _router;
                std::vector<uint8_t> _requestData;
                MetaInfo _metaInfo(Context::kDoIP);
                CancellationHandler _cancellationHandler(false);

                std::future<OperationOutput> _response{
                    _router.Route(
                        _requestData,
                        _metaInfo,
                        std::move(_cancellationHandler))};

                EXPECT_TRUE(_response.valid());
                EXPECT_THROW(_response.get(), std::invalid_argument);
            }

            TEST(UdsServiceRouterTest, RemoveServiceMethod)
            {
                const uint8_t cSid{0x21};
                UdsServiceRouter _router;

                EXPECT_NO_THROW(_router.RemoveService(cSid));
            }
        }
    }
}