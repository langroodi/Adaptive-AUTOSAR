#include "./option.h"

namespace ara
{
    namespace com
    {
        namespace option
        {
            Option::Option(OptionType type, bool discardable) noexcept : mType{type},
                                                                         mDiscardable{discardable}
            {
            }

            OptionType Option::Type() const noexcept
            {
                return mType;
            }

            bool Option::Discardable() const noexcept
            {
                return mDiscardable;
            }

            std::vector<uint8_t> Option::Payload()
            {
                std::vector<uint8_t> _result;

                helper::Inject(_result, Length());

                uint8_t _type = static_cast<uint8_t>(mType);
                _result.push_back(_type);

                const uint8_t _discardableFlag = static_cast<uint8_t>(mDiscardable);
                _result.push_back(_discardableFlag);

                return _result;
            }
        }
    }
}