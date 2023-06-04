#ifndef PROFILE_H
#define PROFILE_H

#include <cstdint>
#include <vector>

namespace ara
{
    namespace com
    {
        /// @brief End to End communication protection protocol namespace
        /// @note The entities within the namespace are NOT fully compatible with the ARA standard.
        namespace e2e
        {
            enum class CheckStatusType
            {
                kOk,           ///!< CRC check is OK
                kWrongCrc,     ///!< Computed CRC does not match the message CRC
                kRepeated,     ///!< The message has been already received
                kNoNewData,    ///!< The message data payload is empty
                kWrongSequence ///!< The message counter difference is negative
            };

            /// @brief E2E protection profile abstract class
            class Profile
            {
            protected:
                Profile() noexcept = default;

            public:
                virtual ~Profile() noexcept = default;

                /// @brief Try to protect a message payload by inserting a CRC header
                /// @param[in] unprotectedData Message payload without any protection
                /// @param[out] protectedData Message payload containing the CRC protection
                /// @returns True if the protected data is generated successfully; otherwise false
                /// @remarks The 'protectedData' should be ignored in case of getting 'false' as the return value.
                /// @remarks The profile internal state (e.g., counter) can be changed in case of getting 'true' as the return value.
                virtual bool TryProtect(
                    const std::vector<uint8_t> &unprotectedData,
                    std::vector<uint8_t> &protectedData) = 0;

                /// @brief Try to protect a message payload by inserting a CRC header while replicating the E2E status
                /// @param[in] unprotectedData Message payload without any protection
                /// @param[out] protectedData Message payload containing the CRC protection
                /// @returns True if the protected data is generated successfully; otherwise false
                /// @remarks The 'protectedData' should be ignored in case of getting 'false' as the return value.
                /// @remarks The profile internal state (e.g., counter) can be changed in case of getting 'true' as the return value.
                /// @see TryProtect
                virtual bool TryForward(
                    const std::vector<uint8_t> &unprotectedData,
                    std::vector<uint8_t> &protectedData) = 0;

                /// @brief Check whether a message is received correctly or not
                /// @param protectedData Message payload containing the CRC protection to be checked
                /// @returns The result of checking the message protection
                /// @remarks The profile internal state (e.g., counter delta) can be changed after calling the function.
                virtual CheckStatusType Check(
                    const std::vector<uint8_t> &protectedData) = 0;
            };
        }
    }
}

#endif