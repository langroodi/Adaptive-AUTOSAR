#include "./request_transfer_exit.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            const uint8_t RequestTransferExit::cSid;

            RequestTransferExit::RequestTransferExit(
                const core::InstanceSpecifier &specifier,
                TransferData &transferData) noexcept : RoutableUdsService(specifier, cSid),
                                                       mTransferData{transferData}
            {
            }

            std::future<OperationOutput> RequestTransferExit::HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler)
            {
                OperationOutput _response;
                std::promise<OperationOutput> _resultPromise;

                bool _succeed{mTransferData.TryResetTransferConfiguration()};

                if (_succeed)
                {
                    // Insert the SID with the increment to the start of the positive response
                    auto _sid{static_cast<uint8_t>(cSid + cPositiveResponseSidIncrement)};
                    _response.responseData.insert(_response.responseData.begin(), _sid);
                }
                else
                {
                    GenerateNegativeResponse(_response, cRequestSequenceError);
                }

                _resultPromise.set_value(_response);
                std::future<OperationOutput> _result{_resultPromise.get_future()};

                return _result;
            }
        }
    }
}