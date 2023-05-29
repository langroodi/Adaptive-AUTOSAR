#include <stdexcept>
#include "./elementary_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            ElementarySupervision::ElementarySupervision(
                TypeOfSupervision supervisionType) noexcept : cSupervisionType{supervisionType},
                                                              mStatus{SupervisionStatus::kDeactivated},
                                                              mOnStatusChanged{nullptr}
            {
            }

            void ElementarySupervision::Report(SupervisionStatus status)
            {
                if (mStatus != status)
                {
                    switch (status)
                    {
                    case SupervisionStatus::kDeactivated:
                    case SupervisionStatus::kExpired:
                        mStatus = status;
                        if (mOnStatusChanged)
                        {
                            mOnStatusChanged(mStatus);
                        }
                        break;

                    case SupervisionStatus::kOk:
                    case SupervisionStatus::kFailed:
                        // Transition from the expired status is only allowed to the deactivated status.
                        if (mStatus != SupervisionStatus::kExpired)
                        {
                            mStatus = status;
                            if (mOnStatusChanged)
                            {
                                mOnStatusChanged(mStatus);
                            }
                        }

                        break;

                    default:
                        throw std::out_of_range("The reported status is not handled!");
                    }
                }
            }

            SupervisionStatus ElementarySupervision::GetStatus() const noexcept
            {
                return mStatus;
            }

            TypeOfSupervision ElementarySupervision::GetType() const noexcept
            {
                return cSupervisionType;
            }

            void ElementarySupervision::SetCallback(
                std::function<void(SupervisionStatus)> &&callback)
            {
                mOnStatusChanged = std::move(callback);
            }
        }
    }
}