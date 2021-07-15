#include "argument.h"
#include <sstream>

template <typename T>
constexpr Argument::Argument(
    T &&t,
    const char *indetifier,
    const char *unit) : mPlayload(t)
{
    mIdentifier(identifier);
    mUnit(unit);
}

constexpr std::string Argument::ToString() const
{
    std::stringstream _ss;
    _ss << mIdentifier << ": " << mPayload << " " << mUnit;
    std::string _result = _ss.str();

    return _result;
}