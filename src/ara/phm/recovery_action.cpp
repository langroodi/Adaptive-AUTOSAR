#include "./recovery_action.h"

namespace ara
{
    namespace phm
    {
        RecoveryAction::RecoveryAction(
            const core::InstanceSpecifier &instance) : mInstance{instance},
                                                       mOffered{false}
        {
        }

        RecoveryAction::RecoveryAction(
            RecoveryAction &&ra) noexcept : mInstance{std::move(ra.mInstance)},
                                            mOffered{ra.mOffered}
        {
        }

        bool RecoveryAction::IsOffered() const noexcept
        {
            return mOffered;
        }

        core::Result<void> RecoveryAction::Offer()
        {
            mOffered = true;
            core::Result<void> _result;
            return _result;
        }

        void RecoveryAction::StopOffer() noexcept
        {
            mOffered = false;
        }
    }
}