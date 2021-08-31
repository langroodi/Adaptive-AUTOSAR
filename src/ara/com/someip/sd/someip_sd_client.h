#ifndef SOMEIP_SD_CLIENT
#define SOMEIP_SD_CLIENT

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /// @brief Service discovery client machine state
                enum class SdClientState
                {
                    ServiceNotSeen,         ///!< Client's service not requested and not seen
                    ServiceSeen,            ///!< Client's service not requsted, but seen
                    RequestedButNotReady,   ///!< Client's service requested, but not ready
                    ServiceReady,           ///!< Client's service is ready
                    Stopped,                ///!< Client stopped
                    InitialWaitPhase,       ///!< Client is in initial waiting phase
                    InitialWaitTimerSet,    ///!< Client is in initial waiting timer set
                    RepetitionPhase,        ///!< Client is in repetition phase
                    RepetitionTimerSet      ///!< Client is in repetition timer set
                };
            }
        }
    }
}

#endif