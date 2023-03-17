#include "./uds_service_router.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            void UdsServiceRouter::AddService(RoutableUdsService *service)
            {
                mServices[service->GetSid()] = service;
            }

            void UdsServiceRouter::RemoveService(uint8_t sid)
            {
                auto _serviceItr{mServices.find(sid)};

                if (_serviceItr != mServices.end())
                {
                    mServices.erase(_serviceItr);
                }
            }

            std::future<OperationOutput> UdsServiceRouter::Route(
                std::uint8_t sid,
                const std::vector<std::uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler)
            {
                auto _serviceIterator{mServices.find(sid)};

                if (_serviceIterator != mServices.end() && _serviceIterator->second->IsOffered())
                {
                    // If the service is added to the router and it has been offered,
                    // route the request to the service.
                    return _serviceIterator->second->HandleMessage(
                        requestData, metaInfo, std::move(cancellationHandler));
                }
                else
                {
                    // Otherwise create a negative UDS response that the service is not supported.
                    OperationOutput _operationOutput;
                    _operationOutput.responseData =
                        std::vector<uint8_t>{
                            cNegativeResponseSid, sid, cServiceNotSupportedNrc};

                    std::promise<OperationOutput> _resultPromise;
                    std::future<OperationOutput> _result{_resultPromise.get_future()};
                    _resultPromise.set_value(_operationOutput);

                    return _result;
                }
            }
        }
    }
}