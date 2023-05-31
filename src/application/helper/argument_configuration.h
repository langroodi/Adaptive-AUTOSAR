#ifndef ARGUMENT_CONFIGURATION_H
#define ARGUMENT_CONFIGURATION_H

#include <map>
#include <string>

namespace application
{
    namespace helper
    {
        /// @brief A helper class to manage the arguments passed to the main application
        class ArgumentConfiguration
        {
        private:
            std::map<std::string, std::string> mArguments;

            bool trySetEchoMode(bool enabled);
            bool tryAskSafely(std::string message, std::string argumentKey);

        public:
            /// @brief Execution manifest filename argument key
            static const std::string cConfigArgument;
            /// @brief Extended Vehicle AA manifest filename argument key
            static const std::string cEvConfigArgument;
            /// @brief Diagnostic Manager manifest filename argument key
            static const std::string cDmConfigArgument;
            /// @brief Platform Health Management manifest filename argument key
            static const std::string cPhmConfigArgument;
            /// @brief VCC API key argument key
            static const std::string cApiKeyArgument;
            /// @brief OAuth 2.0 bearer token argument key
            static const std::string cBearerTokenArgument;

            /// @brief Constructor
            /// @param argc Argument count
            /// @param argv Passed arguments
            /// @param defaultConfigFile Default execution manifest file path
            /// @param extendedVehicleConfigFile Default Extended Vehicle AA manifest file path
            /// @param diagnosticManagerConfigFile Default DM manifest file path
            /// @param healthMonitoringConfigFile Default PHM manifest file path
            ArgumentConfiguration(
                int argc,
                char *argv[],
                std::string defaultConfigFile = "../../configuration/execution_manifest.arxml",
                std::string extendedVehicleConfigFile = "../../configuration/extended_vehicle_manifest.arxml",
                std::string diagnosticManagerConfigFile = "../../configuration/diagnostic_manager_manifest.arxml",
                std::string healthMonitoringConfigFile = "../../configuration/health_monitoring_manifest.arxml");
            ArgumentConfiguration() = delete;

            /// @brief Arguments property getter
            /// @return All the parsed and/or set arguments
            const std::map<std::string, std::string> &GetArguments() const noexcept;

            /// @brief Try asking the user to enter the VCC API key safely
            /// @param message Message to be shown on the console to ask the API key
            /// @return True if the API key is set correctly; otherwise false
            bool TryAskingVccApiKey(
                std::string message = "Please enter the VCC API key:");

            /// @brief Try asking the user to enter the OAuth 2.0 bearer token safely
            /// @param message Message to be shown on the console to ask the  bearer token
            /// @return True if the bearer token is set correctly; otherwise false
            bool TryAskingBearToken(
                std::string message = "Please enter the OAuth 2.0 bearer token:");
        };
    }
}

#endif