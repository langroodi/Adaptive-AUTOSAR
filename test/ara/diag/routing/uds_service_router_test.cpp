#include <gtest/gtest.h>
#include "../../../../src/ara/diag/routing/uds_service_router.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            TEST(UdsServiceRouterTest, RouteMethod)
            {
                const uint8_t cExpectedResult{0x21};
                const size_t cSidIndex{1};

                ara::core::InstanceSpecifier _specifier("Instance0");
                UdsServiceRouter _router(_specifier);
                std::vector<uint8_t> _requestData;
                MetaInfo _metaInfo(Context::kDoIP);
                CancellationHandler _cancellationHandler(false);

                std::future<OperationOutput> _response{
                    _router.Route(
                        cExpectedResult,
                        _requestData,
                        _metaInfo,
                        std::move(_cancellationHandler))};

                EXPECT_TRUE(_response.valid());
                OperationOutput _operationOutput{_response.get()};
                uint8_t _actualResult{_operationOutput.responseData.at(cSidIndex)};

                EXPECT_EQ(cExpectedResult, _actualResult);
            }

            TEST(UdsServiceRouterTest, RemoveServiceMethod)
            {
                const uint8_t cSid{0x21};

                ara::core::InstanceSpecifier _specifier("Instance0");
                UdsServiceRouter _router(_specifier);

                EXPECT_NO_THROW(_router.RemoveService(cSid));
            }
        }
    }
}