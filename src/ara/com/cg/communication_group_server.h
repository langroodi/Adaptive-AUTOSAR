#ifndef COMMUNICATION_GROUP_SERVER_H
#define COMMUNICATION_GROUP_SERVER_H

#include <stdint.h>
#include <functional>
#include <future>
#include <vector>

namespace ara
{
    namespace com
    {
        namespace cg
        {
            /// @brief Response message handler type
            /// @tparam R Response message type
            template <typename R>
            using ResponseHandler = std::function<void(uint32_t, R)>;

            /// @brief Communication group server skeleton
            /// @tparam T Request message type
            /// @tparam R Response message type
            template <typename T, typename R>
            class CommunicationGroupServer
            {
            private:
                ResponseHandler<R> mResponseHandler;
                std::vector<uint32_t> mClients;

            public:
                /// @brief Constructor
                /// @param responseHandler On response message received handler
                explicit CommunicationGroupServer(ResponseHandler<R> responseHandler) : mResponseHandler{responseHandler}
                {
                }

                ~CommunicationGroupServer() noexcept = default;

                /// @brief Broadcast a request message to all the clients
                /// @param msg Request message to be broadcasted
                /// @returns Fire and forget future
                std::future<void> Broadcast(const T &msg) const;

                /// @brief Send a request message to a specific client
                /// @param clientID Communication group client ID
                /// @param msg Request message to be sent
                /// @returns Fire and forget future
                std::future<void> Message(uint32_t clientID, const T &msg) const;

                /// @brief Receive a response message from a client
                /// @param clientID Communication group client ID
                /// @param responseMsg Received response message
                void Response(uint32_t clientID, const R &responseMsg)
                {
                    mResponseHandler(clientID, responseMsg);
                }

                /// @brief List all the subscribed clients
                /// @returns Subscribed clients list future
                std::future<std::vector<uint32_t>> ListClients() const;
            };
        }
    }
}

#endif