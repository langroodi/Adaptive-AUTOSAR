#include <doiplib/diag_message_ack.h>
#include "./diag_message_handler.h"

namespace application
{
    namespace doip
    {
        DiagMessageHandler::DiagMessageHandler(
            helper::CurlWrapper *curl,
            std::string resourcesUrl,
            uint8_t protocolVersion) : mService(curl, resourcesUrl),
                                       cProtocolVersion{protocolVersion}
        {
            mRouter.AddService(&mService);
            mService.Offer();
        }

        DoipLib::Message *DiagMessageHandler::GetMessage()
        {
            return static_cast<DoipLib::Message *>(&mRequest);
        }

        bool DiagMessageHandler::TryHandle(
            const DoipLib::Message *request,
            std::vector<uint8_t> &response) const
        {
            auto _diagMessage{
                dynamic_cast<const DoipLib::DiagMessage *>(request)};

            if (_diagMessage)
            {
                std::vector<uint8_t> _userData;
                _diagMessage->GetUserData(_userData);
                ara::diag::MetaInfo _metaInfo(ara::diag::Context::kDoIP);
                ara::diag::CancellationHandler _cancellationHandler(false);

                try
                {
                    std::future<ara::diag::OperationOutput> _operationOutput{
                        mRouter.Route(
                            _userData, _metaInfo, std::move(_cancellationHandler))};

                    /// @note Previous message field is used for the UDS response data
                    DoipLib::DiagMessageAck _diagMessageAck(
                        cProtocolVersion,
                        _diagMessage->GetSourceAddress(),
                        _diagMessage->GetTargetAddress(),
                        _operationOutput.get().responseData);

                    _diagMessageAck.Serialize(response);

                    return true;
                }
                catch (const std::invalid_argument)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
}