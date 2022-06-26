#include "../diag_error_domain.h"
#include "./routable_uds_service.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            RoutableUdsService::RoutableUdsService(
                const ara::core::InstanceSpecifier &specifier,
                uint8_t sid) noexcept : Specifier{specifier},
                                        mSid{sid},
                                        mOffered{false}
            {
            }

            ara::core::Result<void> RoutableUdsService::Offer()
            {
                if (mOffered)
                {
                    ara::core::ErrorDomain *_errorDomain{DiagErrorDomain::GetDiagDomain()};
                    auto _diagErrorDomain{static_cast<DiagErrorDomain *>(_errorDomain)};
                    ara::core::ErrorCode _errorCode{_diagErrorDomain->MakeErrorCode(DiagErrc::kAlreadyOffered)};
                    auto _result{ara::core::Result<void>::FromError(_errorCode)};

                    return _result;
                }
                else
                {
                    mOffered = true;

                    if (mNotifier)
                    {
                        mNotifier(mSid, mOffered);
                    }

                    ara::core::Result<void> _result;
                    return _result;
                }
            }

            void RoutableUdsService::SetOfferNotifier(std::function<void(uint8_t, bool)> notifier)
            {
                mNotifier = notifier;
            }

            void RoutableUdsService::StopOffer()
            {
                if (mOffered)
                {
                    mOffered = false;

                    if (mNotifier)
                    {
                        mNotifier(mSid, mOffered);
                    }
                }
            }

            RoutableUdsService::~RoutableUdsService()
            {
                StopOffer();
            }
        }
    }
}