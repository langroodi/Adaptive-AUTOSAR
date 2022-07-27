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
            const uint8_t TransferData::cInitialBlockSequenceCounter;

            TransferData::TransferData(
                const ara::core::InstanceSpecifier &specifier) noexcept : RoutableUdsService(specifier, cSid),
                                                                          mTransferDirection{TransferDirection::kNone},
                                                                          mCurrentBlockSequenceCounter{cInitialBlockSequenceCounter},
                                                                          mNextBlockSequenceCounter{cInitialBlockSequenceCounter}
            {
            }

            bool TransferData::tryValidateSequenceCounter(
                OperationOutput &response, uint8_t counter) const
            {
                if (counter == mCurrentBlockSequenceCounter ||
                    counter == mNextBlockSequenceCounter)
                {
                    return true;
                }
                else
                {
                    GenerateNegativeResponse(response, cWrongBlockSequenceCounter);
                    return false;
                }
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
                    _result = tryValidateSequenceCounter(_response, blockSequenceCounter);

                    if (_result)
                    {
                        std::copy(transferRequestParameterRecord.cbegin(),
                                  transferRequestParameterRecord.cbegin() + mMemorySize,
                                  mMemoryPool.begin() + mMemoryAddress);
                    }

                    break;

                case TransferDirection::kUpload:
                    _result = tryValidateSequenceCounter(_response, blockSequenceCounter);

                    if (_result)
                    {
                        _response.responseData =
                            std::vector<uint8_t>(
                                mMemoryPool.cbegin() + mMemoryAddress,
                                mMemoryPool.cbegin() + mMemoryAddress + mMemorySize);
                    }

                    break;

                default:
                    GenerateNegativeResponse(_response, cRequestSequenceError);
                    _result = false;
                }

                if (_result)
                {
                    // Echo the block sequence counter
                    _response.responseData.insert(
                        _response.responseData.begin(), blockSequenceCounter);
                    // Insert the SID with the increment to the start of the positive response
                    auto _sid{static_cast<uint8_t>(cSid + cPositiveResponseSidIncrement)};
                    _response.responseData.insert(_response.responseData.begin(), _sid);
                }
                responsePromise.set_value(_response);

                return _result;
            }

            std::future<OperationOutput> TransferData::HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler)
            {
                std::promise<OperationOutput> _resultPromise;

                if (requestData.size() > cSequenceCounterIndex)
                {
                    uint8_t _blockSeqenceCounter{requestData.at(cSequenceCounterIndex)};
                    std::vector<std::uint8_t> _transferRequestParameterRecord(
                        requestData.cbegin() + cRequestParameterOffset, requestData.cend());

                    bool _succeed{tryTransfer(
                        _blockSeqenceCounter,
                        _transferRequestParameterRecord,
                        _resultPromise)};

                    if (_succeed)
                    {
                        // Expect the current received counter or the incremented by one next time
                        mCurrentBlockSequenceCounter = _blockSeqenceCounter;
                        mNextBlockSequenceCounter = _blockSeqenceCounter + 1;
                    }
                }
                else
                {
                    OperationOutput _response;
                    GenerateNegativeResponse(_response, cIncorrectMessageLength);
                    _resultPromise.set_value(_response);
                }

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

                if ((memorySize == 0) || ((memoryAddress + memorySize) > cMemoryPoolSize))
                {
                    // Memory size is zero or it exceeds the memory pool.
                    return false;
                }

                if (mTransferDirection != TransferDirection::kNone)
                {
                    // A transfer direction has been already set.
                    return false;
                }

                mTransferDirection = transferDirection;
                mMemoryAddress = memoryAddress;
                mMemorySize = memorySize;

                return true;
            }

            bool TransferData::TryResetTransferConfiguration() noexcept
            {
                if (mTransferDirection == TransferDirection::kNone)
                {
                    return false;
                }
                else
                {
                    mTransferDirection = TransferDirection::kNone;
                    mCurrentBlockSequenceCounter = cInitialBlockSequenceCounter;
                    mNextBlockSequenceCounter = cInitialBlockSequenceCounter;

                    return true;
                }
            }
        }
    }
}