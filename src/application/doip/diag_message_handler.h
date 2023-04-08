#ifndef DIAG_MESSAGE_HANDLER_H
#define DIAG_MESSAGE_HANDLER_H

#include <doiplib/message_handler.h>
#include <doiplib/diag_message.h>
#include "../helper/read_data_by_identifier.h"
#include "../../ara/diag/routing/uds_service_router.h"

namespace application
{
    /// @brief Namespace that contains helpers which utilize applications for DoIP communication
    namespace doip
    {
        /// @brief A class to handle UDS messages via DoIP
        class DiagMessageHandler : public DoipLib::MessageHandler
        {
        private:
            DoipLib::DiagMessage mRequest;
            ara::diag::routing::UdsServiceRouter mRouter;
            helper::ReadDataByIdentifier mService;
            const uint8_t cProtocolVersion;

        public:
            /// @brief Constructor
            /// @param curl Configured CURL instance for RESTful communication
            /// @param resourcesUrl Connected vehicle resources access RESTful URL
            /// @param protocolVersion DoIP protocol version
            DiagMessageHandler(
                helper::CurlWrapper *curl,
                std::string resourcesUrl,
                uint8_t protocolVersion);

            DoipLib::Message *GetMessage() override;

            bool TryHandle(
                const DoipLib::Message *request,
                std::vector<uint8_t> &response) const override;
        };
    }
}

#endif