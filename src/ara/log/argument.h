#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <type_traits>

namespace ara
{
    namespace log
    {
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
            Argument(
                T &&t,
                const char *identifier,
                const char *unit) : mPayload(t),
                                    mIdentifier(identifier),
                                    mUnit(unit)
            {
            }

            ~Argument() noexcept = default;
            
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