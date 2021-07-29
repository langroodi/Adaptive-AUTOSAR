#include <gtest/gtest.h>
#include "../../../src/ara/log/logging_framework.h"

namespace ara
{
    namespace log
    {
        TEST(LoggingFrameworkTest, FactoryException)
        {
            const std::string cAppId{"APP01"};
            const LogMode cLogMode{LogMode::kFile};

            ASSERT_THROW(
                LoggingFramework::Create(cAppId, cLogMode),
                std::invalid_argument);
        }
    }
}