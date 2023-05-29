#ifndef ALIVE_SUPERVISION_H
#define ALIVE_SUPERVISION_H

#include <atomic>
#include <chrono>
#include <future>
#include "./elementary_supervision.h"

namespace ara
{
    namespace phm
    {
        /// @brief Namepsace that contains different supervision configurations and algorithms
        /// @note The namespace is not part of the ARA standard.
        namespace supervisors
        {
            /// @brief Supervision method to check an entity aliveness periodically
            class AliveSupervision : public ElementarySupervision
            {
            private:
                static const TypeOfSupervision cSupervisionType;
                
                const uint16_t mExpectedAliveIndicationsMin;
                const uint16_t mExpectedAliveIndicationsMax;
                const uint8_t mFailedReferenceCyclesTolerance;

                uint8_t mAliveCounter;
                std::atomic_bool mRunning;
                std::atomic_uint16_t mAliveIndications;
                std::future<void> mFuture;

                void supervise(std::chrono::milliseconds aliveReferenceCycle);

            public:
                /// @brief Constructor
                /// @param expectedAliveIndications Expected number of checkpoint reports within a certain period
                /// @param minMargin Negative deviation from the expected checkpoint report number
                /// @param maxMargin Positive deviation from the expected checkpoint report number
                /// @param aliveReferenceCycle Time window to check the number of reported checkpoints
                /// @param failedReferenceCyclesTolerance Maximum allowed number of failures
                /// @throws std::invalid_argument Thrown if the alive supervision configuration is invalid
                AliveSupervision(
                    uint16_t expectedAliveIndications,
                    uint16_t minMargin,
                    uint16_t maxMargin,
                    std::chrono::milliseconds aliveReferenceCycle,
                    uint8_t failedReferenceCyclesTolerance);

                AliveSupervision() = delete;
                ~AliveSupervision();

                /// @brief Report the expected checkpoint
                void ReportAliveness() noexcept;
            };
        }
    }
}

#endif