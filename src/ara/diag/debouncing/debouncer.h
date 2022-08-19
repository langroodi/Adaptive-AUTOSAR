#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include <functional>

namespace ara
{
    namespace diag
    {
        /// @brief Namespace of debouncing mechanism components
        /// @note The namespace is not part of the ARA standard.
        namespace debouncing
        {
            /// @brief Monitored event status
            enum class EventStatus : int8_t
            {
                kPassed = -1,   ///!< Finally healed event
                kPending = 0,   ///!< Pending status event
                kFailed = 1     ///!< Finally defective event
            };

            /// @brief Debouncing mechanism interface
            class Debouncer
            {
            protected:
                /// @brief Delegate to be called when the event status is changed via the debouncing
                std::function<void(bool)> OnEventStatusChanged;

                /// @brief Constructor
                /// @param callback Callback to be triggered at the monitored event status change
                explicit Debouncer(std::function<void(bool)> callback) : OnEventStatusChanged{callback}
                {
                }

            public:
                /// @brief Report pre-passed of the monitored event
                virtual void ReportPrepassed() = 0;

                /// @brief Report pre-failed of the monitored event
                virtual void ReportPrefailed() = 0;

                /// @brief Freeze the debouncing mechanism
                virtual void Freeze() = 0;

                /// @brief Reset the debouncing mechanism
                virtual void Reset() = 0;

                virtual ~Debouncer() noexcept = default;
            };
        }
    }
}

#endif