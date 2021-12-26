#ifndef EXECUTION_CLIENT_H
#define EXECUTION_CLIENT_H

#include "../core/instance_specifier.h"
#include "../core/result.h"
#include "./helper/fifo_layer.h"

namespace ara
{
    namespace exec
    {
        /// @brief Adaptive application internal state
        enum class ExecutionState : uint8_t
        {
            kRunning = 0 ///!< Application process performs normally
        };

        /// @brief Class that enables an adaptive application to interact with Execution Management
        class ExecutionClient final
        {
        public:
            /// @brief Message type communicate via the IPC pipe
            using FifoMessageType = std::pair<core::InstanceSpecifier, ExecutionState>;

        private:
            core::InstanceSpecifier mInstanceSpecifier;
            helper::FifoLayer<FifoMessageType> *mCommunicationLayer;

        public:
            /// @brief Constructor
            /// @param instanceSpecifier Adaptive application instance shortname-path
            /// @param communicationLayer IPC communication abstraction layer
            ExecutionClient(
                core::InstanceSpecifier instanceSpecifier,
                helper::FifoLayer<FifoMessageType> *communicationLayer);

            ~ExecutionClient() noexcept = default;

            /// @brief Report the application internal state to Execution Management
            /// @param state Application current internal state
            /// @returns Void Result if the state reporting was successful, otherwise a Result containing the occurred error
            ara::core::Result<void> ReportExecutionState(
                ExecutionState state) const;
        };
    }
}

#endif