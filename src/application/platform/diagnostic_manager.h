#ifndef DIAGNOSTIC_MANAGER_H
#define DIAGNOSTIC_MANAGER_H

#include <asyncbsdsocket/poller.h>
#include "../../ara/exec/helper/modelled_process.h"
#include "../../ara/com/someip/sd/someip_sd_client.h"
#include "../helper/network_configuration.h"

namespace application
{
    namespace platform
    {
        /// @brief Diagnostic Manager (DM) functional cluster
        class DiagnosticManager : public ara::exec::helper::ModelledProcess
        {
        private:
            static const std::string cAppId;

            AsyncBsdSocketLib::Poller mPoller;
            ara::com::helper::NetworkLayer<ara::com::someip::sd::SomeIpSdMessage> *mNetworkLayer;
            ara::com::someip::sd::SomeIpSdClient *mSdClient;

            void configureNetworkLayer(const arxml::ArxmlReader &reader);
            
            void configureSdClient(const arxml::ArxmlReader &reader);

        protected:
            int Main(
                const std::atomic_bool *cancellationToken,
                const std::map<std::string, std::string> &arguments) override;

        public:
            DiagnosticManager();
            ~DiagnosticManager() override;
        };
    }
}

#endif