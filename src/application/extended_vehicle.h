#ifndef EXTENDED_VEHICLE_H
#define EXTENDED_VEHICLE_H

#include <asyncbsdsocket/poller.h>
#include "../ara/exec/helper/modelled_process.h"
#include "../ara/com/someip/sd/someip_sd_server.h"

namespace application
{
    /// @brief Volvo extended vehicle adaptive application
    class ExtendedVehicle : public ara::exec::helper::ModelledProcess
    {
    private:
        static const std::string cAppId;

        AsyncBsdSocketLib::Poller mPoller;
        ara::com::helper::NetworkLayer<ara::com::someip::sd::SomeIpSdMessage> *mNetworkLayer;
        ara::com::someip::sd::SomeIpSdServer *mSdServer;

    protected:
        int Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments) override;

    public:
        ExtendedVehicle();
    };
}

#endif