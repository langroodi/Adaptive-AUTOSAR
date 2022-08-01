#include <gtest/gtest.h>
#include "../../../../src/ara/diag/routing/request_transfer.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            class DummyRequestTransfer : public RequestTransfer
            {
            private:
                static core::InstanceSpecifier mSpecifier;
                static const ReentrancyType cReentrancy{ReentrancyType::kNot};
                static const uint8_t cSid{0x35};
                static const TransferDirection cTransferDirection{TransferDirection::kNone};

                TransferData mTransferData;

            public:
                DummyRequestTransfer() : mTransferData(mSpecifier),
                                         RequestTransfer(mSpecifier, cReentrancy, cSid, mTransferData, cTransferDirection)
                {
                }

                std::future<OperationOutput> HandleMessage(
                    const std::vector<uint8_t> &requestData,
                    MetaInfo &metaInfo,
                    CancellationHandler &&cancellationHandler) override
                {
                    std::promise<OperationOutput> _resultPromise;
                    std::logic_error _resultException("Not implemented");
                    auto _resultExceptionPtr{std::make_exception_ptr(_resultException)};
                    _resultPromise.set_exception(_resultExceptionPtr);
                    std::future<OperationOutput> _result{_resultPromise.get_future()};

                    return _result;
                }
            };

            core::InstanceSpecifier DummyRequestTransfer::mSpecifier{"Instance0"};
            const ReentrancyType DummyRequestTransfer::cReentrancy;
            const uint8_t DummyRequestTransfer::cSid;
            const TransferDirection DummyRequestTransfer::cTransferDirection;

            TEST(RequestTransferTest, Constructor)
            {
                EXPECT_THROW(DummyRequestTransfer _dummy, std::invalid_argument);
            }
        }
    }
}