#ifndef STOPPED_STATE_H
#define STOPPED_STATE_H

#include "../../../helper/machine_state.h"
#include "../someip_sd_client.h"

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
                    /// @brief Client state when the service is stopped
                    /// @note The state is not copyable
                    class StoppedState : public helper::MachineState<SdClientState>
                    {
                    protected:
                        void Deactivate(SdClientState nextState) override;

                    public:
                        StoppedState() noexcept;

                        StoppedState(const StoppedState &) = delete;
                        StoppedState &operator=(const StoppedState &) = delete;

                        void Activate(SdClientState previousState) override;

                        /// @brief Inform the state that the client's service is not requested anymore
                        void ServiceNotRequested();
                    };
                }
            }
        }
    }
}

#endif