#include <gtest/gtest.h>
#include "../../../src/ara/diag/cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        class CancellationHandlerTest : public testing::Test
        {
        protected:
            const bool cInitialIsCanceled = false;

            CancellationHandler Handler;
            bool NotifiedeIsCanceled;

        public:
            CancellationHandlerTest() : Handler{cInitialIsCanceled},
                                        NotifiedeIsCanceled{cInitialIsCanceled}
            {
            }

            void IsCanceledChanged()
            {
                NotifiedeIsCanceled = Handler.IsCanceled();
            }
        };

        TEST_F(CancellationHandlerTest, Constructor)
        {
            bool _actualResult{Handler.IsCanceled()};
            EXPECT_EQ(cInitialIsCanceled, _actualResult);
        }

        TEST_F(CancellationHandlerTest, SetIsCanceledMethod)
        {
            const bool cExpectedResult{!cInitialIsCanceled};

            Handler.SetIsCanceled(cExpectedResult);
            bool _actualResult{Handler.IsCanceled()};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST_F(CancellationHandlerTest, SetNotifierMethod)
        {
            const bool cExpectedResult{!cInitialIsCanceled};

            auto _callback{std::bind(&CancellationHandlerTest::IsCanceledChanged, this)};
            Handler.SetNotifier(_callback);
            Handler.SetIsCanceled(cExpectedResult);
            bool _actualResult{NotifiedeIsCanceled};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }
    }
}