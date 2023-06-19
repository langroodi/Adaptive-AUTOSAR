#include <stdexcept>
#include "./profile11.h"

namespace ara
{
    namespace com
    {
        namespace e2e
        {
            constexpr std::size_t Profile11::cTableSize;
            volatile std::atomic_bool Profile11::mCrcTableInitialized{false};
            std::array<uint8_t, Profile11::cTableSize> Profile11::mCrcTable;

            Profile11::Profile11() noexcept : mProtectingCounter{0},
                                              mCheckingCounter{0}
            {
                const uint8_t cCrcPoly{0x1d};

                if (!mCrcTableInitialized)
                {
                    mCrcTableInitialized = true;
                    initializeCrcTable(cCrcPoly);
                }
            }

            void Profile11::initializeCrcTable(uint8_t crcPoly) noexcept
            {
                const uint8_t cCrcMask{0x80};

                for (uint16_t i = 0; i < cTableSize; ++i)
                {
                    auto _crc{static_cast<uint8_t>(i)};

                    for (uint8_t j = 0; j < 8; ++j)
                    {
                        _crc = (_crc & cCrcMask) ? ((_crc << 1) ^ crcPoly) : (_crc << 1);
                    }

                    mCrcTable[i] = _crc;
                }
            }

            uint8_t Profile11::calculateCrc(
                const std::vector<uint8_t> &data, size_t offset)
            {
                uint8_t _result{0xff};

                for (size_t i = offset; i < data.size(); ++i)
                {
                    const uint8_t cElement{data[i]};
                    const auto cIndex{static_cast<std::size_t>(_result ^ cElement)};
                    _result = mCrcTable[cIndex];
                }

                return ~_result;
            }

            bool Profile11::TryProtect(
                const std::vector<uint8_t> &unprotectedData,
                std::vector<uint8_t> &protectedData)
            {
                const uint8_t cCounterMask{0xf0};

                if (unprotectedData.size() > 0)
                {
                    protectedData = unprotectedData;

                    if (mProtectingCounter < cCounterMax)
                    {
                        ++mProtectingCounter;
                    }
                    else
                    {
                        mProtectingCounter = 0;
                    }

                    const auto cCounter{
                        static_cast<uint8_t>(mProtectingCounter | cCounterMask)};
                    protectedData.insert(protectedData.begin(), cCounter);

                    const uint8_t cCrc{calculateCrc(protectedData)};
                    protectedData.insert(protectedData.begin(), cCrc);

                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool Profile11::TryForward(
                const std::vector<uint8_t> &unprotectedData,
                std::vector<uint8_t> &protectedData)
            {
                throw std::logic_error("The function is not implemented!");
            }

            CheckStatusType Profile11::Check(
                const std::vector<uint8_t> &protectedData)
            {
                const size_t cMinimumSize{3};
                const size_t cCrcOffset{0};
                const size_t cCounterOffset{1};
                const uint8_t cCounterMask{0x0f};

                if (protectedData.size() < cMinimumSize)
                {
                    return CheckStatusType::kNoNewData;
                }

                const uint8_t cReceivedCrc{protectedData[cCrcOffset]};
                const uint8_t cComputedCrc{calculateCrc(protectedData, cCrcOffset + 1)};

                if (cReceivedCrc != cComputedCrc)
                {
                    return CheckStatusType::kWrongCrc;
                }

                const uint8_t cUnmaskedCounter{protectedData[cCounterOffset]};
                const auto cReceivedCounter{
                    static_cast<uint8_t>(cUnmaskedCounter & cCounterMask)};

                CheckStatusType _result;

                const auto cDelta{
                    static_cast<int8_t>(cReceivedCounter - mCheckingCounter)};

                if (cDelta == 0)
                {
                    _result = CheckStatusType::kRepeated;
                }
                else if (mCheckingCounter < cCounterMax && cDelta < 0)
                {
                    _result = CheckStatusType::kWrongSequence;
                }
                else
                {
                    _result = CheckStatusType::kOk;
                }

                mCheckingCounter = cReceivedCounter;

                return _result;
            }
        }
    }
}