#ifndef GLOBAL_SUPERVISION_H
#define GLOBAL_SUPERVISION_H

#include <vector>
#include "./elementary_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            /// @brief Supervision update on global status
            struct SupervisionUpdate
            {
                /// @brief Updated supervision status
                SupervisionStatus status;

                /// @brief Dominant supervision method that determined the global status
                TypeOfSupervision type;
            };

            /// @brief A class to report the global supervision status based on several elementary supervisions
            /// @see ElementarySupervision
            class GlobalSupervision
            {
            private:
                const std::vector<ElementarySupervision *> mSupervisors;

                SupervisionStatus mStatus;
                std::function<void(SupervisionUpdate)> mOnGlobalStatusChanged;

                SupervisionUpdate getGlobalUpdate() const;
                void onElementaryStatusChanged(SupervisionStatus status);

            public:
                /// @brief Constructor
                /// @param supervisors A list of elementary supervision pointers
                explicit GlobalSupervision(
                    std::initializer_list<ElementarySupervision *> supervisors);
                ~GlobalSupervision() noexcept = default;

                /// @brief Get the supervision status
                /// @returns Current global supervision status
                SupervisionStatus GetStatus() const noexcept;

                /// @brief Set a callback to be invoked on the global supervision status change
                /// @param callback On supervision status changed callback
                void SetCallback(
                    std::function<void(SupervisionUpdate)> &&callback);
            };
        }
    }
}

#endif