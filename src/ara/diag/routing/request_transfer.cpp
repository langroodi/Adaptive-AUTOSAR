#include "./request_transfer.h"
#include "./nrc_exception.h"

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
                uint8_t dataFormatIdentifier,
                uint8_t addressAndLengthFormatIdentifier,
                const std::vector<uint8_t> &memoryAddressAndSize,
                size_t &memoryAddress,
                size_t &memorySize) const
            {
                const uint8_t cDataFormatIdentifier{0x00};
                const size_t cExpectedMemoryAddressLength{2};
                const size_t cExpectedMemorySizeLength{2};
                const uint8_t cNibbleMask{0x0f};
                const size_t cByteBitLength{8};

                if (dataFormatIdentifier != cDataFormatIdentifier)
                {
                    // Data encryption and/or compression is not supported.
                    return false;
                }

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
                MetaInfo &metaInfo, OperationOutput &response) const
            {
                const auto cLengthFormatIdentifier{
                    static_cast<uint8_t>(sizeof(uint8_t) << cNibbleBitLength)};

                uint8_t _maxNumberOfBlockLength;
                bool _result{
                    TryExtractValue<uint8_t>(
                        metaInfo, cMaxNumberOfBlockLengthKey, _maxNumberOfBlockLength)};

                if (_result)
                {
                    auto _positiveResponseSid{
                        static_cast<uint8_t>(
                            GetSid() + cPositiveResponseSidIncrement)};

                    response.responseData =
                        std::vector<uint8_t>{
                            _positiveResponseSid,
                            cLengthFormatIdentifier,
                            _maxNumberOfBlockLength};

                    return true;
                }
                else
                {
                    return false;
                }
            }

            std::future<void> RequestTransfer::RequestDataTransfer(
                uint8_t dataFormatIdentifier,
                uint8_t addressAndLengthFormatIdentifier,
                std::vector<uint8_t> memoryAddressAndSize,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler)
            {
                std::promise<void> _resultPromise;

                uint8_t _nrc;
                size_t _memoryAddress;
                size_t _memorySize;

                bool _succeed{
                    TryParseLengthFormat(
                        dataFormatIdentifier,
                        addressAndLengthFormatIdentifier,
                        memoryAddressAndSize,
                        _memoryAddress, _memorySize)};

                try
                {
                    if (_succeed)
                    {
                        _succeed = TryRequestTransfer(_memoryAddress, _memorySize);

                        if (!_succeed)
                        {
                            NrcExecption _exception(cUploadDownloadNotAccepted);
                            throw _exception;
                        }
                    }
                    else
                    {
                        NrcExecption _exception(cRequestOutOfRange);
                        throw _exception;
                    }

                    _resultPromise.set_value();
                }
                catch (const NrcExecption &ex)
                {
                    _resultPromise.set_exception(std::current_exception());
                }

                std::future<void> _result{_resultPromise.get_future()};
                return _result;
            }

            std::future<OperationOutput> RequestTransfer::HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler)
            {
                std::promise<OperationOutput> _resultPromise;
                OperationOutput _response;

                uint8_t _nrc;
                uint8_t _dataFormatIdentifier;
                uint8_t _addressAndLengthFormatIdentifier;
                std::vector<uint8_t> _memoryAddressAndSize;

                bool _succeed{
                    TryParseRequest(
                        requestData,
                        _dataFormatIdentifier,
                        _addressAndLengthFormatIdentifier,
                        _memoryAddressAndSize)};

                if (_succeed)
                {
                    std::future<void> _resquestResultFuture{
                        RequestDataTransfer(
                            _dataFormatIdentifier,
                            _addressAndLengthFormatIdentifier,
                            _memoryAddressAndSize,
                            metaInfo,
                            std::move(cancellationHandler))};

                    try
                    {
                        _resquestResultFuture.get();
                        _succeed = TryGeneratePositiveResponse(metaInfo, _response);

                        if (!_succeed)
                        {
                            _nrc = cUploadDownloadNotAccepted;
                        }
                    }
                    catch (const NrcExecption &ex)
                    {
                        _succeed = false;
                        _nrc = ex.GetNrc();
                    }
                }
                else
                {
                    _nrc = cIncorrectMessageLength;
                }

                if (!_succeed)
                {
                    GenerateNegativeResponse(_response, _nrc);
                }

                _resultPromise.set_value(_response);
                std::future<OperationOutput> _result{_resultPromise.get_future()};
                return _result;
            }
        }
    }
}