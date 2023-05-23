#include "./global_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            GlobalSupervision::GlobalSupervision(
                std::initializer_list<ElementarySupervision *> supervisors) : mSupervisors{supervisors},
                                                                              mOnGlobalStatusChanged{nullptr}
            {
                mStatus =
                    mSupervisors.size() > 0 ? getGlobalStatus() : SupervisionStatus::kDeactivated;

                for (auto supervisor : mSupervisors)
                {
                    auto _callback{
                        std::bind(
                            &GlobalSupervision::onElementaryStatusChanged,
                            this, std::placeholders::_1)};
                    supervisor->SetCallback(std::move(_callback));
                }
            }

            SupervisionStatus GlobalSupervision::getGlobalStatus() const
            {
                SupervisionStatus _result{mSupervisors[0]->GetStatus()};

                for (auto itr = mSupervisors.cbegin() + 1; itr != mSupervisors.cend(); ++itr)
                {
                    SupervisionStatus cNewStatus{(*itr)->GetStatus()};
                    if (cNewStatus > _result)
                    {
                        _result = cNewStatus;
                    }
                }

                return _result;
            }

            void GlobalSupervision::onElementaryStatusChanged(SupervisionStatus status)
            {
                const SupervisionStatus cStatus{getGlobalStatus()};

                if (cStatus != mStatus)
                {
                    mStatus = cStatus;

                    if (mOnGlobalStatusChanged)
                    {
                        mOnGlobalStatusChanged(mStatus);
                    }
                }
            }

            SupervisionStatus GlobalSupervision::GetStatus() const noexcept
            {
                return mStatus;
            }

            void GlobalSupervision::SetCallback(
                std::function<void(SupervisionStatus)> &&callback)
            {
                mOnGlobalStatusChanged = std::move(callback);
            }
        }
    }
}