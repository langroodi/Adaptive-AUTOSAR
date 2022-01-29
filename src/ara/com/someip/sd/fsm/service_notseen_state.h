#ifndef SERVICE_NOTSEEN_STATE_H
#define SERVICE_NOTSEEN_STATE_H

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
                    /// @brief Client's service state when the service is not seen
                    /// @details The state happens when the client's service is not requested and also the server's service is down.
                    /// @note The state is not copyable
                    class ServiceNotseenState : public ClientServiceState
                    {
                    private:
                        std::condition_variable *const mConditionVariable;
                        bool mDisposing;
                        bool mEverRequested;

                        helper::SdClientState waitForNextState();

                    protected:
                        void Deactivate(helper::SdClientState nextState) override;

                    public:
                        /// @brief Constructor
                        /// @param ttlTimer Finite machine state global TTL timer pointer
                        /// @param conditionVariable Condition variable to be notified when the state activated
                        ServiceNotseenState(
                            helper::TtlTimer *ttlTimer,
                            std::condition_variable *conditionVariable) noexcept;

                        ServiceNotseenState() = delete;
                        ServiceNotseenState(const ServiceNotseenState &) = delete;
                        ServiceNotseenState &operator=(const ServiceNotseenState &) = delete;

                        void Activate(helper::SdClientState previousState) override;

                        /// @brief Request service client for the first time
                        /// @note Due to blocking, this function should be called from a separate thread.
                        void RequestService();

                        /// @brief Dispose the state to end the blocking caused by ServiceRequested
                        /// @remarks The side effect of this function call is irreversible.
                        /// @see ServiceRequested
                        void Dispose() noexcept;
                    };
                }
            }
        }
    }
}

#endif