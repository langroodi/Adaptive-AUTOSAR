#ifndef DOIP_CONTROLLER_H
#define DOIP_CONTROLLER_H

#include <map>
#include "./announcement_timer.h"
#include "./controller_config.h"
#include "./message_handler.h"

namespace DoipLib
{
    /// @brief DoIP controller to route and handle DoIP messages
    class DoipController
    {
    private:
        const std::size_t cPayloadTypeOffset{2};

        ControllerConfig mConfiguration;
        AnnouncementTimer mTimer;
        std::map<PayloadType, MessageHandler *> mHandlers;

        void CreateGenericNack(
            GenericNackType nackCode, std::vector<uint8_t> &response) const;

    public:
        /// @brief Constructor
        /// @param configuration Controller configurtation
        DoipController(ControllerConfig &&configuration);

        /// @brief Register a message handler to manage a specific payload type
        /// @param payloadType Payload type to be managed via the handler
        /// @param messageHandler Handler to manage the messages with a specific payload type
        void Register(PayloadType payloadType, MessageHandler *messageHandler);

        /// @brief Try to handle a DoIP request
        /// @param[in] request DoIP request byte vector
        /// @param[out] response DoIP response byte vector
        /// @return True if the request is handled correctly, otherwise false
        /// @note In case of incorrect request handling, the passed response vector will be untouched.
        bool TryHandle(
            const std::vector<uint8_t> &request,
            std::vector<uint8_t> &response) const;

        /// @brief (Re)start initial vehicle announcement
        /// @param callback Delegate to be invoked at each annoucement tick
        /// @note The function will block until the announcement process starts.
        void StartAnnoucement(std::function<void()> &&callback);
    };
}

#endif