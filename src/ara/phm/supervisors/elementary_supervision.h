#ifndef ELEMENTARY_SUPERVISION_H
#define ELEMENTARY_SUPERVISION_H

#include <cstdint>
#include <functional>

namespace ara
{

    namespace phm
    {
        namespace supervisors
        {
            /// @brief Supervision finite state machine status
            enum class SupervisionStatus : uint8_t
            {
                kDeactivated = 0, ///!< Supervision has not been started or has been terminated
                kOk = 1,          ///!< Supervision is correct
                kFailed = 2,      ///!< Supervision is incorrect but does not exceed the tolerance
                kExpired =3       ///!< Supervision is incorrect and also exceeding the tolerance
            };

            /// @brief An abstract class the enables a supervision method to report its elementary supervion status
            class ElementarySupervision
            {
            private:
                SupervisionStatus mStatus;
                std::function<void(SupervisionStatus)> mOnStatusChanged;

            protected:
                ElementarySupervision() noexcept;

                /// @brief Report the new supervision status
                /// @param status New status of the supervision method
                void Report(SupervisionStatus status);

            public:
                virtual ~ElementarySupervision() noexcept = default;

                /// @brief Get the supervision status
                /// @returns Current elementary supervision status
                SupervisionStatus GetStatus() const noexcept;

                /// @brief Set a callback to be invoked on the supervision status change
                /// @param callback On supervision status changed callback
                void SetCallback(
                    std::function<void(SupervisionStatus)> &&callback);
            };
        }
    }
}

#endif