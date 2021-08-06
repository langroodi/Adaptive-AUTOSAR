#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <type_traits>

namespace ara
{
    namespace log
    {
        /// @brief A payload (quantity) with an unit wrapper
        /// @tparam T Payload type
        template <typename T>
        class Argument
        {
        private:
            T mPayload;
            std::string mIdentifier;
            std::string mUnit;
            const std::string cIdSeperator{": "};
            const std::string cUnitSeperator{" "};

        public:
            /// @brief brief description Constructor
            /// @param payload Payload value
            /// @param identifier Payload ID
            /// @param unit Playload unit
            Argument(
                T &&payload,
                const char *identifier,
                const char *unit) : mPayload(payload),
                                    mIdentifier(identifier),
                                    mUnit(unit)
            {
            }

            ~Argument() noexcept = default;
            
            /// @brief Convert the payload to a standard string
            /// @returns Serialized payload string
            std::string ToString() const
            {
                std::string _payloadString = std::to_string(mPayload);
                std::string _result =
                    mIdentifier + cIdSeperator + _payloadString + cUnitSeperator + mUnit;

                return _result;
            }
        };
    }
}

#endif