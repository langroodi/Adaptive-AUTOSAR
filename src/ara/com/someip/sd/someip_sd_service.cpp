#include <stdexcept>
#include <random>
#include "./someip_sd_message.h"

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
                    double initialDelayMin,
                    double initialDelayMax,
                    double repetitionBaseDelay,
                    double cycleOfferDelay,
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
                                             mLinkAvailabe{false},
                                             mRepetitionDelay{repetitionDelay},
                                             mCycleOfferDelay{cycleOfferDelay},
                                             mRepetitionMax{repetitionMax},

                {
                    if (repetionBaseDelay < 0)
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

                    std::default_random_engine _generator;
                    std::uniform_real_distribution<double> _distribution(
                        initialDelayMin, initialDelayMax);
                    mInitialDelay = _generator(_distribution);
                }

                void SomeIpSdServer::SetServiceAvailability(bool available) noexcept
                {
                    mServiceAvailable = available;
                }
            }
        }
    }
}