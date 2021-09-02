#include <stdexcept>
#include <random>
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
                    double initialDelayMin,
                    double initialDelayMax,
                    double repetitionBaseDelay,
                    uint32_t repetitionMax,
                    uint16_t sdPort,
                    bool serviceAvailable) : mSdIpAddress{sdIpAddress},
                                             mSdPort{sdPort},
                                             mState{cInitialState},
                                             mRepetitionBaseDelay{repetitionBaseDelay},
                                             mRepetitionCounter{repetitionMax}
                {
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
                    mInitialDelay = _distribution(_generator);
                }
            }
        }
    }
}