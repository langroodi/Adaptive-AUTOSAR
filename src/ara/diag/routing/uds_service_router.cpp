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
                const std::vector<std::uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) const
            {
                const size_t cSidIndex{0};

                std::promise<OperationOutput> _resultPromise;
                std::future<OperationOutput> _result{_resultPromise.get_future()};

                if (requestData.size() > cSidIndex)
                {
                    const uint8_t cSid{requestData.at(cSidIndex)};
                    auto _serviceIterator{mServices.find(cSid)};

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
                                cNegativeResponseSid, cSid, cServiceNotSupportedNrc};

                        _resultPromise.set_value(_operationOutput);
                    }
                }
                else
                {
                    std::exception_ptr _exceptionPtr{
                        std::make_exception_ptr(
                            std::invalid_argument("Empty request data"))};
                    _resultPromise.set_exception(_exceptionPtr);
                }

                return _result;
            }
        }
    }
}