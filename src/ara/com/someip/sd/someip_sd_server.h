#ifndef SOMEIP_SD_SERVER
#define SOMEIP_SD_SERVER

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief Service discovery server machine state
                enum class SdServerState
                {
                    NotReady,               ///!< Server is down
                    InitialWaitPhase,       ///!< Server is in initial waiting phase
                    InitialWaitTimerSet,    ///!< Server is in initial waiting timer set
                    RepetitionPhase,        ///!< Server is in repetition phase
                    RepetitionTimerSet,     ///!< Server is in repetition timer set
                    MainPhase,              ///!< Server is in main phase
                    MainTimerSet            ///!< Server is in main timer set
                };
            }
        }
    }
}

#endif