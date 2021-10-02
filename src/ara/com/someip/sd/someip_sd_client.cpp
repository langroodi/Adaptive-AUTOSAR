#include <stdexcept>
#include "./someip_sd_client.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                SomeIpSdClient::SomeIpSdClient(
                    helper::Ipv4Address sdIpAddress,
                    int initialDelayMin,
                    int initialDelayMax,
                    int repetitionBaseDelay,
                    uint32_t repetitionMax,
                    uint16_t sdPort,
                    bool serviceRequested) : mSdIpAddress{sdIpAddress},
                                             mSdPort{sdPort},
                                             mState{cInitialState},
                                             mInitialDelayMin{initialDelayMin},
                                             mInitialDelayMax{initialDelayMax},
                                             mRepetitionBaseDelay{repetitionBaseDelay},
                                             mRepetitionCounter{repetitionMax},
                                             mServiceRequested{serviceRequested}
                {
                    if ((initialDelayMin < 0) ||
                        (initialDelayMax < 0) ||
                        (initialDelayMin < initialDelayMax))
                    {
                        throw std::invalid_argument(
                            "Invalid initial delay minimum and/or maximum.");
                    }
                }

                void SomeIpSdClient::RequestService(bool requested) noexcept
                {
                    mServiceRequested = requested;
                }
            }
        }
    }
}