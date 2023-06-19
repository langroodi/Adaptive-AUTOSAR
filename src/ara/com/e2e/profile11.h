#ifndef PROFILE11_H
#define PROFILE11_H

#include <array>
#include <atomic>
#include "./profile.h"

namespace ara
{
    namespace com
    {
        namespace e2e
        {
            /// @brief E2E Profile11 variant A implementation
            /// @remarks The CRC calculation is based on CRC table without any hardware acceleration.
            class Profile11 : public Profile
            {
            private:
                static constexpr std::size_t cTableSize{256};
                static volatile std::atomic_bool mCrcTableInitialized;
                static std::array<uint8_t, cTableSize> mCrcTable;

                const uint8_t cCounterMax{0x0e};

                uint8_t mProtectingCounter;
                uint8_t mCheckingCounter;

                static void initializeCrcTable(uint8_t crcPoly) noexcept;
                static uint8_t calculateCrc(
                    const std::vector<uint8_t> &data, std::size_t offset = 0);

            public:
                Profile11() noexcept;

                bool TryProtect(
                    const std::vector<uint8_t> &unprotectedData,
                    std::vector<uint8_t> &protectedData) override;

                bool TryForward(
                    const std::vector<uint8_t> &unprotectedData,
                    std::vector<uint8_t> &protectedData) override;

                CheckStatusType Check(
                    const std::vector<uint8_t> &protectedData) override;
            };
        }
    }
}

#endif