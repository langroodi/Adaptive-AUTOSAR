#ifndef NOTREADY_STATE_H
#define NOTREADY_STATE_H

#include "../../../helper/machine_state.h"
#include "../someip_sd_server.h"

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
                    /// @brief Machine state when the server's service is down or stopped
                    /// @note The state is not copyable
                    class NotReadyState : public helper::MachineState<SdServerState>
                    {
                    protected:
                        void Deactivate(SdServerState nextState) override;

                    public:
                        NotReadyState();

                        NotReadyState(const NotReadyState &) = delete;
                        NotReadyState &operator=(const NotReadyState &) = delete;

                        void Activate(SdServerState previousState) override;

                        /// @brief Inform the state that the server's service is up
                        void ServiceActivated();
                    };
                }
            }
        }
    }
}

#endif