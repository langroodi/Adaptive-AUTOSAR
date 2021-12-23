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
    }
}