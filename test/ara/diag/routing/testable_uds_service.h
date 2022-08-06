#ifndef TESTABLE_UDS_SERVICE_H
#define TESTABLE_UDS_SERVICE_H

#include <gtest/gtest.h>
#include "../../../../src/ara/diag/routing/routable_uds_service.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            class TestableUdsService : public testing::Test
            {
            protected:
                MetaInfo GeneralMetaInfo;

                TestableUdsService() noexcept : GeneralMetaInfo(Context::kDoIP)
                {
                }

                bool TryGetNrc(
                    RoutableUdsService *service,
                    const std::vector<uint8_t> &requestData,
                    uint8_t &nrc)
                {
                    const size_t cRejectedSidIndex{1};
                    const size_t cNrcIndex{2};

                    CancellationHandler _cancellationHandler(false);

                    std::future<OperationOutput> _responseFuture{
                        service->HandleMessage(
                            requestData,
                            GeneralMetaInfo,
                            std::move(_cancellationHandler))};

                    OperationOutput _response{_responseFuture.get()};

                    uint8_t _sid{_response.responseData.at(cRejectedSidIndex)};
                    if (_sid == service->GetSid())
                    {
                        nrc = _response.responseData.at(cNrcIndex);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            };
        }
    }
}

#endif