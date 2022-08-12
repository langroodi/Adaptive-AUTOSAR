#include "generic_routine.h"

namespace ara
{
    namespace diag
    {
        const uint8_t GenericRoutine::cSid;

        GenericRoutine::GenericRoutine(
            const core::InstanceSpecifier &specifier,
            ReentrancyType reentrancyType) : routing::RoutableUdsService(specifier, cSid),
                                             mReentrancy{reentrancyType}
        {
        }

        std::future<OperationOutput> GenericRoutine::HandleMessage(
            const std::vector<uint8_t> &requestData,
            MetaInfo &metaInfo,
            CancellationHandler &&cancellationHandler)
        {
            const size_t cSubFunctionIndex{1};
            const size_t cRoutineIdMsbIndex{2};
            const size_t cRoutineIdLsbIndex{3};

            const uint8_t cStartSubFunction{0x01};
            const uint8_t cStopSubFunction{0x02};
            const uint8_t cResultRequestSubFunction{0x03};

            OperationOutput _response;

            if (requestData.size() > cRoutineIdLsbIndex)
            {
                uint8_t _subFunction{requestData.at(cSubFunctionIndex)};

                auto _routineId{
                    static_cast<uint16_t>(requestData.at(cRoutineIdMsbIndex) << 8)};
                _routineId |= requestData.at(cRoutineIdLsbIndex);
                
                switch (_subFunction)
                {
                case cStartSubFunction:
                    return Start(
                        _routineId,
                        requestData,
                        metaInfo,
                        std::move(cancellationHandler));

                case cStopSubFunction:
                    return Stop(
                        _routineId,
                        requestData,
                        metaInfo,
                        std::move(cancellationHandler));

                case cResultRequestSubFunction:
                    return RequestResults(
                        _routineId,
                        requestData,
                        metaInfo,
                        std::move(cancellationHandler));
                
                default:
                    GenerateNegativeResponse(_response, cSubFunctionNotSupported);
                }
            }
            else
            {
                GenerateNegativeResponse(_response, cIncorrectMessageLength);
            }

            std::promise<OperationOutput> _resultPromise;
            std::future<OperationOutput> _result{_resultPromise.get_future()};
            _resultPromise.set_value(_response);

            return _result;
        }
    }
}