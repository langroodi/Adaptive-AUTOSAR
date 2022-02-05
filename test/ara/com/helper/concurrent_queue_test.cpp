#include <gtest/gtest.h>
#include "../../../../src/ara/com/helper/concurrent_queue.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            TEST(ConcurrentQueueTest, Constructor)
            {
                ConcurrentQueue<bool> _queue;
                EXPECT_TRUE(_queue.Empty());
            }

            TEST(ConcurrentQueueTest, QueuingScenario)
            {
                bool _expectedResult{true};
                ConcurrentQueue<bool> _queue;
                bool _enqueued = _queue.TryEnqueue(std::move(_expectedResult));
                EXPECT_TRUE(_enqueued);
                EXPECT_FALSE(_queue.Empty());

                bool _actualResult{false};
                bool _dequeued = _queue.TryDequeue(_actualResult);
                EXPECT_TRUE(_dequeued);
                EXPECT_TRUE(_queue.Empty());

                EXPECT_EQ(_expectedResult, _actualResult);
            }
        }
    }
}