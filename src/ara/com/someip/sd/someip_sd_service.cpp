#include <stdexcept>
#include "./someip_sd_server.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                SomeIpSdServer::SomeIpSdServer(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint32_t minorVersion,
                    helper::Ipv4Address sdIpAddress,
                    int initialDelayMin,
                    int initialDelayMax,
                    int repetitionBaseDelay,
                    int cycleOfferDelay,
                    uint32_t repetitionMax,
                    uint16_t sdPort,
                    bool serviceAvailable) : mServiceId{serviceId},
                                             mInstanceId{instanceId},
                                             mMajorVersion{majorVersion},
                                             mMinorVersion{minorVersion},
                                             mSdIpAddress{sdIpAddress},
                                             mSdPort{sdPort},
                                             mState{cInitialState},
                                             mServiceAvailable{serviceAvailable},
                                             mLinkAvailable{false},
                                             mInitialDelayMin{initialDelayMin},
                                             mInitialDelayMax{initialDelayMax},
                                             mRepetitionBaseDelay{repetitionBaseDelay},
                                             mCycleOfferDelay{cycleOfferDelay},
                                             mRepetitionCounter{repetitionMax}
                {
                    if (repetitionBaseDelay < 0)
                    {
                        throw std::invalid_argument(
                            "Invalid repetition base delay.");
                    }

                    if (cycleOfferDelay < 0)
                    {
                        throw std::invalid_argument(
                            "Invalid cyclic offer delay.");
                    }

                    if ((initialDelayMin < 0) ||
                        (initialDelayMax < 0) ||
                        (initialDelayMin < initialDelayMax))
                    {
                        throw std::invalid_argument(
                            "Invalid initial delay minimum and/or maximum.");
                    }
                }

                void SomeIpSdServer::SetServiceAvailability(bool available) noexcept
                {
                    mServiceAvailable = available;
                }
            }
        }
    }
}