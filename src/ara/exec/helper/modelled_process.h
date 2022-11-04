#ifndef MODELLED_PROCESS_H
#define MODELLED_PROCESS_H

namespace ara
{
    namespace exec
    {
        namespace helper
        {
            /// @brief A class that models an instance of an Adaptive (Platform) Application executable
            class ModelledProcess
            {
            public:
                /// @brief Initialize the process model
                virtual void Initialize() = 0;

                /// @brief Terminate the process model
                virtual void Terminate() = 0;

                virtual ~ModelledProcess() noexcept = default;
            };
        }
    }
}

#endif