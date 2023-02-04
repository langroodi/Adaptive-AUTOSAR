#ifndef SOMEIP_SD_AGENT_H
#define SOMEIP_SD_AGENT_H

#include <future>
#include "../../helper/finite_state_machine.h"
#include "../../helper/network_layer.h"
#include "./someip_sd_message.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief SOME/IP service discovery agent (i.e., a server or a client)
                /// @tparam T Agent state enumeration type
                template <typename T>
                class SomeIpSdAgent
                {
                protected:
                    /// @brief Agent's FSM
                    helper::FiniteStateMachine<T> StateMachine;

                    /// @brief Agent running state future object
                    std::future<void> Future;

                    /// @brief Network communication abstraction layer
                    helper::NetworkLayer<SomeIpSdMessage> *CommunicationLayer;

                    /// @brief Start the service discovery agent
                    /// @param state Current FSM state before start
                    virtual void StartAgent(T state) = 0;

                    /// @brief Stop the service discovery agent
                    /// @param state Current FSM state before stop
                    virtual void StopAgent() = 0;

                public:
                    /// @brief Constructor
                    /// @param networkLayer Network communication abstraction layer
                    SomeIpSdAgent(
                        helper::NetworkLayer<SomeIpSdMessage> *networkLayer) : CommunicationLayer{networkLayer}
                    {
                    }

                    /// @brief Start the service discovery agent
                    void Start()
                    {
                        // Valid future means the timer is not expired yet.
                        if (Future.valid())
                        {
                            throw std::logic_error(
                                "The state has been already activated.");
                        }
                        else
                        {
                            T _state = GetState();
                            StartAgent(_state);
                        }
                    }

                    /// @brief Get the current server state
                    /// @returns Server machine state
                    T GetState() const noexcept
                    {
                        return StateMachine.GetState();
                    }

                    /// @brief Join to the timer's thread
                    void Join()
                    {
                        // If the future is valid, block unitl its result becomes avialable after the timer expiration.
                        if (Future.valid())
                        {
                            Future.get();
                        }
                    }

                    /// @brief Stop the service discovery agent
                    /// @note It is safe to recall the function if the agent has been already stopped.
                    void Stop()
                    {
                        StopAgent();
                    }

                    virtual ~SomeIpSdAgent() noexcept = default;
                };
            }
        }
    }
}

#endif