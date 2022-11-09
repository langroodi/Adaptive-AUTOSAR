#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "./message.h"

namespace DoipLib
{
    /// @brief Callback wrapper to handle a DoIP message type at its reception
    class MessageHandler
    {
    public:
        /// @brief Get an instance of the message for request handling
        /// @return Message instance that request byte vector is mapped into
        virtual Message *GetMessage() = 0;

        /// @brief Try to handle a request
        /// @param[in] request Request to be handled
        /// @param[out] response Response byte vector
        /// @return True if the request is handled correctly, otherwise false
        /// @note In case of incorrect request handling, the passed response vector will be untouched.
        virtual bool TryHandle(
            const Message *request, std::vector<uint8_t> &response) const = 0;

        virtual ~MessageHandler() noexcept = default;
    };
}

#endif