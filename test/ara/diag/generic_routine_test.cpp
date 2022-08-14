#include "./routing/testable_uds_service.h"
#include "../../../src/ara/diag/generic_routine.h"

namespace ara
{
    namespace diag
    {
        static const ara::core::InstanceSpecifier cSpecifier("Instance0");

        class GenericRoutineTest : public GenericRoutine, public routing::TestableUdsService
        {
        private:
            static const ReentrancyType cReentrancy{ReentrancyType::kNot};

            std::future<OperationOutput> GetDummyResponse()
            {
                OperationOutput _dummyResponse;
                std::promise<OperationOutput> _resultPromise;
                std::future<OperationOutput> _result{_resultPromise.get_future()};
                _resultPromise.set_value(_dummyResponse);

                return _result;
            }

        public:
            GenericRoutineTest() : GenericRoutine(cSpecifier, cReentrancy)
            {
            }

            std::future<OperationOutput> Start(
                uint16_t routineId,
                std::vector<uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) override
            {
                return GetDummyResponse();
            }

            std::future<OperationOutput> Stop(
                uint16_t routineId,
                std::vector<uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) override
            {
                return GetDummyResponse();
            }

            std::future<OperationOutput> RequestResults(
                uint16_t routineId,
                std::vector<uint8_t> requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) override
            {
                return GetDummyResponse();
            }
        };

        const ReentrancyType GenericRoutineTest::cReentrancy;

        TEST_F(GenericRoutineTest, IncorrectRequestScenario)
        {
            const uint8_t cExpectedNrc{0x13};

            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{GetSid()};
            bool _hasNrc{TryGetNrc(this, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(GenericRoutineTest, InvalidSubFunctionScenario)
        {
            const uint8_t cExpectedNrc{0x12};
            const uint8_t cSubFunction{0xff};
            const uint8_t cRoutineIdMsb{0x02};
            const uint8_t cRoutineIdLsb{0x01};

            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{
                GetSid(), cSubFunction, cRoutineIdMsb, cRoutineIdLsb};
            bool _hasNrc{TryGetNrc(this, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }
    }
}