#include <algorithm>
#include "./transfer_data.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            const uint8_t TransferData::cSid;
            const size_t TransferData::cMemoryPoolSize;

            TransferData::TransferData(
                const ara::core::InstanceSpecifier &specifier) noexcept : RoutableUdsService(specifier, cSid),
                                                                          mTransferDirection{TransferDirection::kNone}
            {
            }

            bool TransferData::tryTransfer(
                uint8_t blockSequenceCounter,
                const std::vector<std::uint8_t> &transferRequestParameterRecord,
                std::promise<OperationOutput> &responsePromise)
            {
                bool _result;
                OperationOutput _response;

                switch (mTransferDirection)
                {
                case TransferDirection::kDownload:
                    std::copy(transferRequestParameterRecord.cbegin(),
                              transferRequestParameterRecord.cbegin() + mMemorySize,
                              mMemoryPool.begin() + mMemoryAddress);

                    _result = true;
                    break;

                case TransferDirection::kUpload:
                    _response.responseData =
                        std::vector<uint8_t>(
                            mMemoryPool.cbegin() + mMemoryAddress,
                            mMemoryPool.cbegin() + mMemoryAddress + mMemorySize);

                    _result = true;
                    break;

                default:
                    _result = false;
                }

                if (_result)
                {
                    // Echo the block sequence counter
                    _response.responseData.insert(
                        _response.responseData.begin(), blockSequenceCounter);
                    // Insert the SID to the start of the positive response
                    _response.responseData.insert(
                        _response.responseData.begin(), cSid);

                    responsePromise.set_value(_response);
                }

                return _result;
            }

            std::future<OperationOutput> TransferData::HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler)
            {
                std::promise<OperationOutput> _resultPromise;
                uint8_t _blockSeqenceCounter{requestData.at(cSequenceCounterIndex)};
                std::vector<std::uint8_t> _transferRequestParameterRecord(
                    requestData.cbegin() + cRequestParameterOffset, requestData.cend());

                tryTransfer(
                    _blockSeqenceCounter,
                    _transferRequestParameterRecord,
                    _resultPromise);

                std::future<OperationOutput> _result{_resultPromise.get_future()};

                return _result;
            }

            bool TransferData::TrySetTransferConfiguration(
                TransferDirection transferDirection,
                size_t memoryAddress,
                size_t memorySize) noexcept
            {
                if (transferDirection == TransferDirection::kNone)
                {
                    // Transfer data direction is not specified.
                    return false;
                }

                if (memoryAddress >= cMemoryPoolSize)
                {
                    // Memory address is larger than the memory pool.
                    return false;
                }

                if ((memoryAddress + memorySize) > cMemoryPoolSize)
                {
                    // Memory size exceeds the memory pool.
                    return false;
                }

                mTransferDirection = transferDirection;
                mMemoryAddress = memoryAddress;
                mMemorySize = memorySize;

                return true;
            }
        }
    }
}