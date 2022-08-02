#include "./request_transfer.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            RequestTransfer::RequestTransfer(
                const ara::core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType,
                uint8_t sid,
                TransferData &transferData,
                TransferDirection transferDirection) : RoutableUdsService(specifier, sid),
                                                       mReentrancy{reentrancyType},
                                                       mTransferData{transferData},
                                                       mTransferDirection{transferDirection != TransferDirection::kNone ? transferDirection : throw std::invalid_argument("Transfer direction should be determined")}
            {
            }

            bool RequestTransfer::TryParseRequest(
                const std::vector<uint8_t> &requestData,
                uint8_t &dataFormatIdentifier,
                uint8_t &addressAndLengthFormatIdentifier,
                std::vector<uint8_t> &memoryAddressAndSize) const
            {
                const size_t cDataFormatIdIndex{1};
                const size_t cAddressAndLengthFormatIdIndex{2};
                const size_t cMemoryAddressAndSizeOffset{3};

                if (requestData.size() > cMemoryAddressAndSizeOffset)
                {
                    dataFormatIdentifier = requestData.at(cDataFormatIdIndex);

                    addressAndLengthFormatIdentifier =
                        requestData.at(cAddressAndLengthFormatIdIndex);

                    auto _memoryAddressAndSizeBeginItr{
                        requestData.cbegin() + cMemoryAddressAndSizeOffset};
                    memoryAddressAndSize =
                        std::vector<uint8_t>(
                            _memoryAddressAndSizeBeginItr, requestData.cend());

                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool RequestTransfer::TryParseLengthFormat(
                uint8_t addressAndLengthFormatIdentifier,
                const std::vector<uint8_t> &memoryAddressAndSize,
                size_t &memoryAddress,
                size_t &memorySize) const
            {
                const size_t cExpectedMemoryAddressLength{2};
                const size_t cExpectedMemorySizeLength{2};
                const size_t cNibbleBitLength{4};
                const uint8_t cNibbleMask{0x0f};
                const size_t cByteBitLength{8};

                auto _actualMemoryAddressLength{
                    static_cast<size_t>(
                        addressAndLengthFormatIdentifier & cNibbleMask)};

                auto _actualMemorySizeLength{
                    static_cast<size_t>(
                        addressAndLengthFormatIdentifier >> cNibbleBitLength)};

                auto _totalLength{
                    static_cast<size_t>(
                        _actualMemoryAddressLength + _actualMemorySizeLength)};

                if ((_actualMemoryAddressLength == cExpectedMemoryAddressLength) &&
                    (_actualMemorySizeLength == cExpectedMemorySizeLength) &&
                    (memoryAddressAndSize.size() == _totalLength))
                {
                    size_t _offset{0};

                    memoryAddress = memoryAddressAndSize.at(_offset) << cByteBitLength;
                    ++_offset;
                    memoryAddress |= memoryAddressAndSize.at(_offset);
                    ++_offset;

                    memorySize = memoryAddressAndSize.at(_offset) << cByteBitLength;
                    ++_offset;
                    memorySize |= memoryAddressAndSize.at(_offset);

                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool RequestTransfer::TryRequestTransfer(
                size_t memoryAddress, size_t memorySize)
            {
                return mTransferData.TrySetTransferConfiguration(
                    mTransferDirection, memoryAddress, memorySize);
            }

            bool RequestTransfer::TryGeneratePositiveResponse(
                MetaInfo &metaInfo, std::vector<uint8_t> &response) const
            {
                uint8_t _maxNumberOfBlockLength;
                bool _result{
                    TryExtractValue<uint8_t>(
                        metaInfo, cMaxNumberOfBlockLengthKey, _maxNumberOfBlockLength)};

                if (_result)
                {
                    auto _positiveResponseSid{
                        static_cast<uint8_t>(
                            GetSid() + cPositiveResponseSidIncrement)};

                    response =
                        std::vector<uint8_t>{
                            _positiveResponseSid, _maxNumberOfBlockLength};

                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
}