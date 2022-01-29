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
                        std::condition_variable *const mConditionVariable;

                        helper::SdClientState waitForNextState();

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

                        void Activate(helper::SdClientState previousState) override;
                    };
                }
            }
        }
    }
}

#endif