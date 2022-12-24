#include <gtest/gtest.h>
#include "../../../src/ara/core/instance_specifier.h"

namespace ara
{
    namespace core
    {
        TEST(InstanceSpecifierTest, Constructor)
        {
            const std::string cMetaModelId{"id"};
            InstanceSpecifier _instanceSpecifier{cMetaModelId};

            EXPECT_TRUE(_instanceSpecifier.ToString() == cMetaModelId);
        }

        TEST(InstanceSpecifierTest, Factory)
        {
            const std::string cMetaModelId{"id"};
            Result<InstanceSpecifier> _result =
                InstanceSpecifier::Create(cMetaModelId);

            EXPECT_TRUE(_result.Value() == cMetaModelId);
        }

        TEST(InstanceSpecifierTest, SerializeMethod)
        {
            const std::string cMetaModelId{"id"};
            const std::vector<uint8_t> cExpectedResult({0, 0, 0, 2, 105, 100});
            const InstanceSpecifier cInstanceSpecifier{cMetaModelId};

            std::vector<uint8_t> _actualResult;
            cInstanceSpecifier.Serialize(_actualResult);

            EXPECT_EQ(cExpectedResult, _actualResult);
        }
    }
}