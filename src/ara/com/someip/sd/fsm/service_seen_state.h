#ifndef SERVICE_SEEN_STATE_H
#define SERVICE_SEEN_STATE_H

#include "./client_service_state.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                namespace fsm
                {
                    /// @brief Client's state when the service is seen (up) but not requested
                    /// @note The state is not copyable.
                    class ServiceSeenState : public ClientServiceState
                    {
                    private:
                        std::condition_variable *mConditionVariable;

                        void onTimerExpired();

                    protected:
                        void Deactivate(helper::SdClientState nextState) override;

                    public:
                        /// @brief Constructor
                        /// @param ttlTimer Finite machine state global TTL timer pointer
                        /// @param conditionVariable Condition variable to be notified when the state activated
                        ServiceSeenState(
                            helper::TtlTimer *ttlTimer,
                            std::condition_variable *conditionVariable) noexcept;

                        ServiceSeenState() = delete;
                        ServiceSeenState(const ServiceSeenState &) = delete;
                        ServiceSeenState &operator=(const ServiceSeenState &) = delete;
                        ~ServiceSeenState() override;

                        void Activate(helper::SdClientState previousState) override;

                        /// @brief Inform the state that the client's service is requested
                        void ServiceRequested();

                        void ServiceOffered(uint32_t ttl) override;

                        /// @brief Inform the state that the service has been stopped
                        void ServiceStopped();
                    };
                }
            }
        }
    }
}

#endif