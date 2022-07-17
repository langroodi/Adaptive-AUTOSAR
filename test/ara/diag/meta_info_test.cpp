#include <gtest/gtest.h>
#include "../../../src/ara/diag/meta_info.h"

namespace ara
{
    namespace diag
    {
        TEST(MetaInfoTest, Constructor)
        {
            const Context cExpectedResult{Context::kDoIP};
            MetaInfo _metaInfo(cExpectedResult);
            Context _actualResult{_metaInfo.GetContext()};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST(MetaInfoTest, GetValueMethod)
        {
            const Context cContext{Context::kDiagnosticCommunication};
            const std::string cValidKey{"kSA"};
            const std::string cInvalidKey{"kTA"};
            const std::string cExpectedValue{"00F0"};

            MetaInfo _metaInfo(cContext);
            _metaInfo.SetValue(cValidKey, cExpectedValue);

            core::Optional<std::string> _validOptionalValue{_metaInfo.GetValue(cValidKey)};
            EXPECT_TRUE(_validOptionalValue.HasValue());

            std::string _actualValue = _validOptionalValue.Value();
            EXPECT_EQ(cExpectedValue, _actualValue);

            core::Optional<std::string> _invalidOptionalValue{_metaInfo.GetValue(cInvalidKey)};
            EXPECT_FALSE(_invalidOptionalValue.HasValue());
        }

        TEST(MetaInfoTest, SetValueMethod)
        {
            const Context cContext{Context::kDiagnosticCommunication};
            const std::string cKey{"kSA"};
            const std::string cExpectedValue{"00F0"};
            const std::string cInvalidValue{"003B"};

            MetaInfo _metaInfo(cContext);
            _metaInfo.SetValue(cKey, cExpectedValue);
            _metaInfo.SetValue(cKey, cInvalidValue);

            core::Optional<std::string> _optionalValue{_metaInfo.GetValue(cKey)};
            EXPECT_TRUE(_optionalValue.HasValue());

            std::string _actualValue = _optionalValue.Value();
            EXPECT_EQ(cExpectedValue, _actualValue);
        }
    }
}