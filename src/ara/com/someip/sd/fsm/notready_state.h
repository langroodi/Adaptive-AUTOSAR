#ifndef NOTREADY_STATE_H
#define NOTREADY_STATE_H

#include "../../../helper/machine_state.h"

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
                    class NotReadyState : public helper::MachineState<helper::SdServerState>
                    {
                    private:
                        const std::function<void()> mOnServiceStopped;

                    protected:
                        void Deactivate(helper::SdServerState nextState) override;

                    public:
                        /// @brief Constructor
                        /// @param onServiceStopped Callback to be invoked when the service is stopped
                        NotReadyState(std::function<void()> onServiceStopped);

                        NotReadyState() = delete;
                        NotReadyState(const NotReadyState &) = delete;
                        NotReadyState &operator=(const NotReadyState &) = delete;

                        void Activate(helper::SdServerState previousState) override;

                        /// @brief Inform the state that the server's service is up
                        void ServiceActivated();
                    };
                }
            }
        }
    }
}

#endif