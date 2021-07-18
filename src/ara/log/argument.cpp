#include "argument.h"

namespace ara
{
    namespace log
    {
        template <typename T>
        Argument<T>::Argument(
            T &&t,
            const char *identifier,
            const char *unit) : mPayload(t),
                                mIdentifier(identifier),
                                mUnit(unit)
        {
        }

        template <typename T>
        std::string Argument<T>::ToString() const
        {
            std::string _payloadString = std::to_string(mPayload);
            std::string _result =
                mIdentifier + cIdSeperator + _payloadString + cUnitSeperator + mUnit;

            return _result;
        }
    }
}
