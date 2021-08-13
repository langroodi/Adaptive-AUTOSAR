#ifndef COMMUNICATION_GROUP_CLIENT_H
#define COMMUNICATION_GROUP_CLIENT_H

#include <stdint.h>
#include <functional>
#include <future>

namespace ara
{
    namespace com
    {
        namespace cg
        {
            /// @brief Request message handler type
            /// @tparam R Request message type
            template <typename T>
            using RequestHandler = std::function<void(T)>;

            /// @brief Communication group client proxy
            /// @tparam T Request message type
            /// @tparam R Response message type
            template <typename T, typename R>
            class CommunicationGroupClient
            {
            private:
                RequestHandler<T> mRequestHandler;

            public:
                /// @brief Constructor
                /// @param requestHandler On request message received handler
                explicit CommunicationGroupClient(RequestHandler<T> requestHandler) : mRequestHandler{requestHandler}
                {
                }

                ~CommunicationGroupClient() noexcept = default;

                /// @brief Receive a request message from the server
                /// @param msg Received request message
                void Message(const T& msg)
                {
                    mRequestHandler(msg);
                }

                /// @brief Send a response message to the server
                /// @param responseMsg Response message to be sent
                /// @returns Fire and forget future
                std::future<void> Response(const R& responseMsg) const;
            };
        }
    }
}
#endif