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
                    mSupervisors.size() > 0 ? getGlobalUpdate().status : SupervisionStatus::kDeactivated;

                for (auto supervisor : mSupervisors)
                {
                    auto _callback{
                        std::bind(
                            &GlobalSupervision::onElementaryStatusChanged,
                            this, std::placeholders::_1)};
                    supervisor->SetCallback(std::move(_callback));
                }
            }

            SupervisionUpdate GlobalSupervision::getGlobalUpdate() const
            {
                SupervisionUpdate _result;
                _result.status = mSupervisors[0]->GetStatus();
                _result.type = mSupervisors[0]->GetType();

                for (auto itr = mSupervisors.cbegin() + 1; itr != mSupervisors.cend(); ++itr)
                {
                    const ElementarySupervision* cElementarySupervision{*itr};
                    SupervisionStatus cNewStatus{cElementarySupervision->GetStatus()};
                    if (cNewStatus > _result.status)
                    {
                        _result.status = cNewStatus;
                        _result.type = cElementarySupervision->GetType();
                    }
                }

                return _result;
            }

            void GlobalSupervision::onElementaryStatusChanged(SupervisionStatus status)
            {
                const SupervisionUpdate cUpdate{getGlobalUpdate()};

                if (cUpdate.status != mStatus)
                {
                    mStatus = cUpdate.status;

                    if (mOnGlobalStatusChanged)
                    {
                        mOnGlobalStatusChanged(cUpdate);
                    }
                }
            }

            SupervisionStatus GlobalSupervision::GetStatus() const noexcept
            {
                return mStatus;
            }

            void GlobalSupervision::SetCallback(
                std::function<void(SupervisionUpdate)> &&callback)
            {
                mOnGlobalStatusChanged = std::move(callback);
            }
        }
    }
}