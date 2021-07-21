#include "./error_code.h"

namespace ara
{
    namespace core
    {
        std::string ErrorCode::ToString() const
        {
            const std::string cSeparator = ": ";
            std::string _domainName{Domain().Name()};            
            std::string _result = _domainName + cSeparator + Message();

            return _result;
        }
    }
}