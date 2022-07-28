#ifndef NRC_EXCEPTION_H
#define NRC_EXCEPTION_H

#include <stdint.h>
#include <exception>

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            /// @brief Negative Response Code Exception
            class NrcExecption : public std::exception
            {
            private:
                const uint8_t mNrc;

            public:
                NrcExecption() = delete;
                /// @brief Constructor
                /// @param nrc Negative Response Code
                explicit NrcExecption(uint8_t nrc) noexcept;
            
                /// @brief Get NRC
                /// @returns Negative response code that the exception contains
                uint8_t GetNrc() const noexcept;
            };
        }
    }
}
#endif