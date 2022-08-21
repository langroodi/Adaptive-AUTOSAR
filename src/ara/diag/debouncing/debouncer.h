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
            enum EventStatus
            {
                kPassed = -1, ///!< Finally healed event
                kPending = 0, ///!< Pending status event
                kFailed = 1   ///!< Finally defective event
            };

            /// @brief Debouncing mechanism interface
            class Debouncer
            {
            private:
                EventStatus mEventStatus;

                std::function<void(bool)> onEventStatusChanged;

            protected:
                /// @brief Constructor
                /// @param callback Callback to be triggered at the monitored event status change
                explicit Debouncer(std::function<void(bool)> callback);

                /// @brief Set the monitored event status
                /// @param status Monitored event new status
                void SetEventStatus(EventStatus status);

            public:
                /// @brief Report pre-passed of the monitored event
                virtual void ReportPrepassed() = 0;

                /// @brief Report finally passed of the monitored event
                virtual void ReportPassed() = 0;

                /// @brief Report pre-failed of the monitored event
                virtual void ReportPrefailed() = 0;

                /// @brief Report finally failed of the monitored event
                virtual void ReportFailed() = 0;

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