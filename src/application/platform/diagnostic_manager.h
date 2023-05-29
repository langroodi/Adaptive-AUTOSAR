#ifndef DIAGNOSTIC_MANAGER_H
#define DIAGNOSTIC_MANAGER_H

#include <obdemulator/serial_communication.h>
#include <obdemulator/obd_emulator.h>
#include "../../ara/exec/helper/modelled_process.h"
#include "../../ara/com/someip/sd/someip_sd_client.h"
#include "../../ara/diag/monitor.h"
#include "../helper/network_configuration.h"
#include "../doip/obd_to_doip_converter.h"

namespace application
{
    namespace platform
    {
        /// @brief Diagnostic Manager (DM) functional cluster
        class DiagnosticManager : public ara::exec::helper::ModelledProcess
        {
        private:
            static const std::string cAppId;
            static const std::string cSerialPort;
            static const speed_t cBaudrate;
            static const bool cSupportExtended;
            static const ObdEmulator::CanBusSpeed cSpeed;

            ara::com::helper::NetworkLayer<ara::com::someip::sd::SomeIpSdMessage> *mNetworkLayer;
            ara::com::someip::sd::SomeIpSdClient *mSdClient;
            const ara::core::InstanceSpecifier *mEventSpecifier;
            ara::diag::Event *mEvent;
            const ara::core::InstanceSpecifier *mMonitorSpecifier;
            ara::diag::Monitor *mMonitor;
            ObdEmulator::SerialCommunication mSerialCommunication;
            ObdEmulator::CanDriver mCanDriver;
            doip::ObdToDoipConverter *mObdToDoipConverter;
            ObdEmulator::ObdEmulator *mObdEmulator;

            void configureNetworkLayer(const arxml::ArxmlReader &reader);

            void configureSdClient(const arxml::ArxmlReader &reader);

            void onEventStatusChanged(ara::diag::EventStatusByte eventStatus);

            void configureEvent(const arxml::ArxmlReader &reader);

            void onInitMonitor(ara::diag::InitMonitorReason reason);

            void configureMonitor(const arxml::ArxmlReader &reader);

            void checkServiceDiscovery();

        protected:
            int Main(
                const std::atomic_bool *cancellationToken,
                const std::map<std::string, std::string> &arguments) override;

        public:
            /// @brief Constructor
            /// @param poller Global poller for network communication
            explicit DiagnosticManager(AsyncBsdSocketLib::Poller *poller);

            ~DiagnosticManager() override;
        };
    }
}

#endif