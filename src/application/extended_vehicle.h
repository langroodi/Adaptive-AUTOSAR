#ifndef EXTENDED_VEHICLE_H
#define EXTENDED_VEHICLE_H

#include "../ara/exec/helper/modelled_process.h"
#include "../ara/com/someip/sd/someip_sd_server.h"
#include "./helper/network_configuration.h"

namespace application
{
    /// @brief Volvo extended vehicle adaptive application
    class ExtendedVehicle : public ara::exec::helper::ModelledProcess
    {
    private:
        static const std::string cAppId;

        ara::com::helper::NetworkLayer<ara::com::someip::sd::SomeIpSdMessage> *mNetworkLayer;
        ara::com::someip::sd::SomeIpSdServer *mSdServer;

        void configureNetworkLayer(const arxml::ArxmlReader &reader);

        helper::NetworkConfiguration getNetworkConfiguration(
            const arxml::ArxmlReader &reader);

        void configureSdServer(const arxml::ArxmlReader &reader);

    protected:
        int Main(
            const std::atomic_bool *cancellationToken,
            const std::map<std::string, std::string> &arguments) override;

    public:
        /// @brief Construcotr
        /// @param poller Global poller for network communication
        ExtendedVehicle(AsyncBsdSocketLib::Poller *poller);

        ~ExtendedVehicle() override;
    };
}

#endif