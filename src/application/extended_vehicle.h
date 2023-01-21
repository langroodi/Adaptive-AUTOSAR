#ifndef EXTENDED_VEHICLE_H
#define EXTENDED_VEHICLE_H

#include <asyncbsdsocket/poller.h>
#include "../ara/com/someip/sd/someip_sd_server.h"
#include "../ara/com/someip/sd/sd_network_layer.h"
#include "../ara/exec/helper/modelled_process.h"

namespace application
{
    /// @brief Volvo extended vehicle adaptive application
    class ExtendedVehicle : public ara::exec::helper::ModelledProcess
    {
    private:
        AsyncBsdSocketLib::Poller mPoller;
        ara::com::someip::sd::SomeIpSdServer *mSdServer;
        helper::NetworkLayer<SomeIpSdMessage> *mNetworkLayer;

    protected:
        int Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments) override;

    public:
        ~ExtendedVehicle() override;
    };
}

#endif