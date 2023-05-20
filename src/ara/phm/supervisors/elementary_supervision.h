#ifndef ELEMENTARY_SUPERVISION_H
#define ELEMENTARY_SUPERVISION_H

#include <functional>

namespace ara
{

    namespace phm
    {
        namespace supervisors
        {
            /// @brief Supervision finite state machine status
            enum class SupervisionStatus
            {
                kDeactivated, ///!< Supervision has not been started or has been terminated
                kOk,          ///!< Supervision is correct
                kFailed,      ///!< Supervision is incorrect but does not exceed the tolerance
                kExpired      ///!< Supervision is incorrect and also exceeding the tolerance
            };

            /// @brief An abstract class the enables a supervision method to report its elementary supervion status
            class ElementarySupervision
            {
            private:
                const std::function<void(SupervisionStatus)> mOnStatusChanged;
                SupervisionStatus mStatus;

            protected:
                /// @brief Constructor
                /// @param callback Callback to be invoked on the supervision status change
                ElementarySupervision(
                    std::function<void(SupervisionStatus)> &&callback);

                /// @brief Report the new supervision status
                /// @param status New status of the supervision method
                void Report(SupervisionStatus status);

            public:
                ElementarySupervision() = delete;
                virtual ~ElementarySupervision() noexcept = default;

                /// @brief Get the supervision status
                /// @returns Current elementary supervision status
                SupervisionStatus GetStatus() const noexcept;
            };
        }
    }
}

#endif