#include "./testable_uds_service.h"
#include "../../../../src/ara/diag/routing/request_transfer_exit.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            class RequestTransferExitTest : public TestableUdsService
            {
            private:
                static core::InstanceSpecifier mSpecifier;

                TransferData mTransferData;

            protected:
                RequestTransferExit Service;

                bool TryBeginTransferData()
                {
                    const TransferDirection cTransferDirection{TransferDirection::kDownload};
                    const size_t cMemoryAddress{512};
                    const size_t cMemorySize{64};

                    bool _result(
                        mTransferData.TrySetTransferConfiguration(
                            cTransferDirection, cMemoryAddress, cMemorySize));

                    return _result;
                }

            public:
                RequestTransferExitTest() : mTransferData(mSpecifier),
                                            Service(mSpecifier, mTransferData)
                {
                }
            };

            core::InstanceSpecifier RequestTransferExitTest::mSpecifier{"Instance0"};

            TEST_F(RequestTransferExitTest, RequestSequenceErrorScenario)
            {
                const uint8_t cExpectedNrc{0x24};
                uint8_t _actualNrc;
                std::vector<uint8_t> _requestData{Service.GetSid()};
                bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

                EXPECT_TRUE(_hasNrc);
                EXPECT_EQ(cExpectedNrc, _actualNrc);
            }

            TEST_F(RequestTransferExitTest, PositiveResponseScenario)
            {
                const size_t cSidIndex{0};
                const uint8_t cPositiveResponseSidIncrement{0x40};
                const auto cExpectedResult{
                    static_cast<uint8_t>(
                        Service.GetSid() + cPositiveResponseSidIncrement)};

                EXPECT_TRUE(TryBeginTransferData());

                std::vector<uint8_t> _requestData{cExpectedResult};
                CancellationHandler _cancellationHandler(false);

                auto _responseFuture{Service.HandleMessage(
                    _requestData,
                    GeneralMetaInfo,
                    std::move(_cancellationHandler))};

                OperationOutput _response{_responseFuture.get()};
                uint8_t _actualResult{_response.responseData.at(cSidIndex)};

                EXPECT_EQ(cExpectedResult, _actualResult);
            }
        }
    }
}