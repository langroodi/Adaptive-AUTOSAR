#include <stdexcept>
#include "./meta_info.h"

namespace ara
{
    namespace diag
    {
        MetaInfo::MetaInfo(Context context) : mContext{context}
        {
        }

        core::Optional<std::string> MetaInfo::GetValue(std::string key)
        {
            core::Optional<std::string> _result;

            try
            {
                _result = mValues.at(key);
            }
            catch (std::out_of_range)
            {
            }

            return _result;
        }

        void MetaInfo::SetValue(std::string key, std::string value)
        {
            std::pair<std::string, std::string> _pair(key, value);
            mValues.insert(_pair);
        }

        Context MetaInfo::GetContext() const noexcept
        {
            return mContext;
        }
    }
}