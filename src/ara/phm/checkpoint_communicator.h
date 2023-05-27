#ifndef CHECKPOINT_COMMUNICATOR_H
#define CHECKPOINT_COMMUNICATOR_H

#include <stdint.h>
#include <functional>

namespace ara
{
    /// @brief Platform Health Monitoring functional cluster namespace
    namespace phm
    {
        /// @brief An abstract class to communicate a checkpoint between an application and the PHM cluster
        /// @note The class is not part of the ARA standard.
        class CheckpointCommunicator
        {
        public:
            /// @brief Callback type for checkpoint reception
            using CheckpointReception = std::function<void(uint32_t)>;

        protected:
            /// @brief Callback to be invoked at a checkpoint reception
            CheckpointReception Callback;

            CheckpointCommunicator() noexcept = default;

        public:
            /// @brief Try to send a checkpoint occurrence
            /// @param checkpoint Occurred checkpoint
            /// @returns True if the checkpoint is successfully queued for sending; otherwise false
            virtual bool TrySend(uint32_t checkpoint) = 0;

            /// @brief Set a callback to be invoked at a checkpoint reception
            /// @param callback Callback to be invoked
            void SetCallback(CheckpointReception &&callback);

            /// @brief Reset the callback to be invoked at a checkpoint reception
            void ResetCallback() noexcept;

            virtual ~CheckpointCommunicator() noexcept;
        };
    }
}

#endif