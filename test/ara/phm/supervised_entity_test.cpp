#include <gtest/gtest.h>
#include "../../../src/ara/phm/supervised_entity.h"
#include "./mocked_checkpoint_communicator.h"

namespace ara
{
    namespace phm
    {
        enum class DummyCheckpoint : uint32_t
        {
            None = 0,
            Startup = 1
        };

        TEST(SupervisedEntityTest, ReportCheckpointMehod)
        {
            const core::InstanceSpecifier cInstance("Instance0");
            const DummyCheckpoint cExpectedResult{DummyCheckpoint::Startup};

            MockedCheckpointCommunicator _communicator;
            DummyCheckpoint _actualResult{DummyCheckpoint::None};
            auto _callback =
                [&](uint32_t checkpointId) noexcept
            { _actualResult = static_cast<DummyCheckpoint>(checkpointId); };
            _communicator.SetCallback(_callback);

            SupervisedEntity _se(cInstance, &_communicator);
            _se.ReportCheckpoint(cExpectedResult);

            EXPECT_EQ(cExpectedResult, _actualResult);
        }
    }
}