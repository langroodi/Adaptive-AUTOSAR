#include <gtest/gtest.h>
#include "../../../src/ara/exec/worker_thread.h"

namespace ara
{
    namespace exec
    {
        TEST(WorkerThreadTest, SingleInstanceUniqueness)
        {
            WorkerThread _workerThread;

            uint64_t _random1 = _workerThread.GetRandom();
            uint64_t _random2 = _workerThread.GetRandom();

            uint64_t _diff = _random2 - _random1;
            EXPECT_GE(_diff, WorkerThread::cOffsetStep);
        }

        TEST(WorkerThreadTest, MultiInstanceUniqueness)
        {
            WorkerThread _workerThread1;
            WorkerThread _workerThread2;

            uint64_t _random1 = _workerThread1.GetRandom();
            uint64_t _random2 = _workerThread2.GetRandom();

            uint64_t _diff = _random2 - _random1;
            EXPECT_GE(_diff, WorkerThread::cOffsetStep);
        }
    }
}