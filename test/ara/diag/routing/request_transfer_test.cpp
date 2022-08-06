#include "./testable_uds_service.h"
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

            TEST(DummyRequestTransfer, Constructor)
            {
                EXPECT_THROW(DummyRequestTransfer _dummy, std::invalid_argument);
            }

            class RequestTransferTest : public RequestTransfer, public TestableUdsService
            {
            private:
                static core::InstanceSpecifier mSpecifier;
                static const ReentrancyType cReentrancy{ReentrancyType::kNot};
                static const TransferDirection cTransferDirection{TransferDirection::kDownload};

                const std::string cMaxNumberOfBlockLengthKey{"MaxNumberOfBlockLength"};

                TransferData mTransferData;

            protected:
                static const uint8_t cSid{0x34};
                const uint8_t cMaxNumberOfBlockLength{64};
                const uint8_t cDataFormatIdentifier{0x00};

            public:
                RequestTransferTest() : mTransferData(mSpecifier),
                                        RequestTransfer(mSpecifier, cReentrancy, cSid, mTransferData, cTransferDirection)

                {
                    std::string _maxNumberOfBlockLengthStr{std::to_string(cMaxNumberOfBlockLength)};
                    GeneralMetaInfo.SetValue(cMaxNumberOfBlockLengthKey, _maxNumberOfBlockLengthStr);
                }
            };

            core::InstanceSpecifier RequestTransferTest::mSpecifier{"Instance0"};
            const ReentrancyType RequestTransferTest::cReentrancy;
            const uint8_t RequestTransferTest::cSid;
            const TransferDirection RequestTransferTest::cTransferDirection;

            TEST_F(RequestTransferTest, ValidRequestParsing)
            {
                const uint8_t cAddressAndLengthFormatIdentifier{0x22};
                const size_t cMemoryAddressAndSizeOffset{3};

                const std::vector<uint8_t> cRequestData{
                    cSid, cDataFormatIdentifier, cAddressAndLengthFormatIdentifier, 0x01, 0x02, 0x00, 0x03};

                uint8_t _dataFormatIdentifier;
                uint8_t _addressAndLengthFormatIdentifier;
                std::vector<uint8_t> _memoryAddressAndSize;

                EXPECT_TRUE(
                    TryParseRequest(
                        cRequestData,
                        _dataFormatIdentifier,
                        _addressAndLengthFormatIdentifier,
                        _memoryAddressAndSize));

                EXPECT_EQ(cDataFormatIdentifier, _dataFormatIdentifier);
                EXPECT_EQ(cAddressAndLengthFormatIdentifier, _addressAndLengthFormatIdentifier);
                EXPECT_TRUE(
                    std::equal(
                        cRequestData.cbegin() + cMemoryAddressAndSizeOffset, cRequestData.cend(),
                        _memoryAddressAndSize.cbegin()));
            }

            TEST_F(RequestTransferTest, InvalidRequestParsing)
            {
                const std::vector<uint8_t> cRequestData{cSid, cDataFormatIdentifier};

                uint8_t _dataFormatIdentifier;
                uint8_t _addressAndLengthFormatIdentifier;
                std::vector<uint8_t> _memoryAddressAndSize;

                EXPECT_FALSE(
                    TryParseRequest(
                        cRequestData,
                        _dataFormatIdentifier,
                        _addressAndLengthFormatIdentifier,
                        _memoryAddressAndSize));
            }

            TEST_F(RequestTransferTest, ValidLengthFormatParsing)
            {
                const uint8_t cAddressAndLengthFormatIdentifier{0x22};
                const std::vector<uint8_t> cMemoryAddressAndSize{0x01, 0x02, 0x00, 0x03};
                const size_t cExpectedAddress{258};
                const size_t cExpectedSize{3};

                size_t _actualAddress;
                size_t _actualSize;

                EXPECT_TRUE(
                    TryParseLengthFormat(
                        cDataFormatIdentifier,
                        cAddressAndLengthFormatIdentifier,
                        cMemoryAddressAndSize,
                        _actualAddress, _actualSize));

                EXPECT_EQ(cExpectedAddress, _actualAddress);
                EXPECT_EQ(cExpectedSize, _actualSize);
            }

            TEST_F(RequestTransferTest, InvalidLengthFormatParsing)
            {
                const uint8_t cValidDataFormatIdentifier{cDataFormatIdentifier};
                const uint8_t cInvalidDataFormatIdentifier{0xff};
                const uint8_t cValidAddressAndLengthFormatId{0x22};
                const uint8_t cValidAddressAndInvalidLengthFormatId{0x21};
                const uint8_t cInvalidAddressAndValidLengthFormatId{0x12};
                const std::vector<uint8_t> cValidMemoryAddressAndSize{0x01, 0x02, 0x00, 0x03};
                const std::vector<uint8_t> cInvalidMemoryAddressAndSize{0x01, 0x02, 0x03};

                size_t _actualAddress;
                size_t _actualSize;

                EXPECT_FALSE(
                    TryParseLengthFormat(
                        cInvalidDataFormatIdentifier,
                        cValidAddressAndLengthFormatId,
                        cValidMemoryAddressAndSize,
                        _actualAddress, _actualSize));

                EXPECT_FALSE(
                    TryParseLengthFormat(
                        cValidDataFormatIdentifier,
                        cValidAddressAndInvalidLengthFormatId,
                        cValidMemoryAddressAndSize,
                        _actualAddress, _actualSize));

                EXPECT_FALSE(
                    TryParseLengthFormat(
                        cValidDataFormatIdentifier,
                        cInvalidAddressAndValidLengthFormatId,
                        cValidMemoryAddressAndSize,
                        _actualAddress, _actualSize));

                EXPECT_FALSE(
                    TryParseLengthFormat(
                        cValidDataFormatIdentifier,
                        cValidAddressAndLengthFormatId,
                        cInvalidMemoryAddressAndSize,
                        _actualAddress, _actualSize));
            }

            TEST_F(RequestTransferTest, PositiveResponseGeneration)
            {
                const size_t cSidIndex{0};
                const size_t cLengthFormatIdIndex{1};
                const size_t cMaxNumberOfBlockLengthIndex{2};
                const auto cExpectedSid{static_cast<uint8_t>(cSid + cPositiveResponseSidIncrement)};
                const uint8_t cExpectedLengthFormat{0x10};

                OperationOutput _response;

                EXPECT_TRUE(TryGeneratePositiveResponse(GeneralMetaInfo, _response));

                uint8_t _actualSid{
                    _response.responseData.at(cSidIndex)};
                EXPECT_EQ(cExpectedSid, _actualSid);

                uint8_t _actualLengthFormat{
                    _response.responseData.at(cLengthFormatIdIndex)};
                EXPECT_EQ(_actualLengthFormat, cExpectedLengthFormat);

                uint8_t _actualMaxNumberOfBlockLength{
                    _response.responseData.at(cMaxNumberOfBlockLengthIndex)};
                EXPECT_EQ(cMaxNumberOfBlockLength, _actualMaxNumberOfBlockLength);
            }

            TEST_F(RequestTransferTest, IncorrectRequestScenario)
            {
                const uint8_t cExpectedNrc{0x13};

                uint8_t _actualNrc;
                std::vector<uint8_t> _requestData{GetSid()};
                bool _hasNrc{TryGetNrc(this, _requestData, _actualNrc)};

                EXPECT_TRUE(_hasNrc);
                EXPECT_EQ(cExpectedNrc, _actualNrc);
            }

            TEST_F(RequestTransferTest, OutOfRangeRequestScenario)
            {
                const uint8_t cExpectedNrc{0x31};

                uint8_t _actualNrc;
                std::vector<uint8_t> _requestData{GetSid(), 0xff, 0x22, 0x00};
                bool _hasNrc{TryGetNrc(this, _requestData, _actualNrc)};

                EXPECT_TRUE(_hasNrc);
                EXPECT_EQ(cExpectedNrc, _actualNrc);
            }

            TEST_F(RequestTransferTest, NotAcceptedRequestScenario)
            {
                const uint8_t cExpectedNrc{0x70};

                uint8_t _actualNrc;
                std::vector<uint8_t> _requestData{GetSid(), 0x00, 0x22, 0xff, 0xff, 0xff, 0xff};
                bool _hasNrc{TryGetNrc(this, _requestData, _actualNrc)};

                EXPECT_TRUE(_hasNrc);
                EXPECT_EQ(cExpectedNrc, _actualNrc);
            }

            TEST_F(RequestTransferTest, AcceptedRequestScenario)
            {
                uint8_t _actualNrc;
                std::vector<uint8_t> _requestData{GetSid(), 0x00, 0x22, 0x00, 0x01, 0x00, 0x01};
                bool _hasNrc{TryGetNrc(this, _requestData, _actualNrc)};

                EXPECT_FALSE(_hasNrc);
            }
        }
    }
}