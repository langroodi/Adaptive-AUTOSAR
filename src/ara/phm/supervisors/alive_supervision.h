#ifndef ALIVE_SUPERVISION_H
#define ALIVE_SUPERVISION_H

#include <atomic>
#include <chrono>
#include <functional>

namespace ara
{
    namespace phm
    {
        /// @brief Namepsace that contains different supervision configurations and algorithms
        /// @note The namespace is not part of the ARA standard.
        namespace supervisors
        {
            /// @brief Supervision method to check an entity aliveness periodically
            class AliveSupervision
            {
            private:
                const uint16_t mExpectedAliveIndicationsMin;
                const uint16_t mExpectedAliveIndicationsMax;
                const uint8_t mFailedReferenceCyclesTolerance;
                const std::function<void(void)> mOnFailedCallback;

                uint8_t mAliveCounter;
                std::atomic_bool mRunning;
                std::atomic_uint16_t mAliveIndications;

                void supervise(std::chrono::milliseconds aliveReferenceCycle);

            public:
                /// @brief Constructor
                /// @param expectedAliveIndications Expected number of checkpoint report within a certain period
                /// @param minMargin Negative deviation from the expected checkpoint report
                /// @param maxMargin Positive deviation from the expected checkpoint report
                /// @param aliveReferenceCycle Time window to check the number of reported checkpoints
                /// @param failedReferenceCyclesTolerance Maximum allowed number of failure
                /// @param onFailedCallback Callback to be invoked if the number of failure exceeds the tolerance
                /// @throws std::invalid_argument Thrown if the negative and/or positive deviations are invalid
                AliveSupervision(
                    uint16_t expectedAliveIndications,
                    uint16_t minMargin,
                    uint16_t maxMargin,
                    std::chrono::milliseconds aliveReferenceCycle,
                    uint8_t failedReferenceCyclesTolerance,
                    std::function<void(void)> &&onFailedCallback);

                AliveSupervision() = delete;
                ~AliveSupervision();

                /// @brief Report the expected checkpoint
                void ReportAliveness() noexcept;
            };
        }
    }
}

#endif