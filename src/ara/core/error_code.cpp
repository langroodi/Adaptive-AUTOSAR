#include "./error_code.h"

namespace ara
{
    namespace core
    {
        std::string ErrorCode::Message() const noexcept
        {
            std::string _result;
            return _result;
        }

        std::string ErrorCode::ToString() const
        {
            std::string _result = Message();

            return _result;
        }
    }
}