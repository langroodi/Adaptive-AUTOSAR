#ifndef FUNCTION_GROUP_STATE_H
#define FUNCTION_GROUP_STATE_H

#include <vector>
#include "./function_group.h"

namespace ara
{
    namespace exec
    {
        /// @brief Class that links a function group to a certain state
        class FunctionGroupState final
        {
        private:
            const FunctionGroup *mFunctionGroup;
            std::string mStateMetaModel;

            FunctionGroupState(
                const FunctionGroup* functionGroup,
                std::string metaModelIdentifier);

        public:
            /// @brief FunctionGroupState factory
            /// @param metaModelIdentifier State manifest of a function group
            /// @returns Result containing a new instance if creation was successful, otherwise a Result containing the occurred error
            static core::Result<FunctionGroupState> Create(
                const FunctionGroup &functionGroup,
                std::string metaModelIdentifier);

            FunctionGroupState() = delete;
            FunctionGroupState(const FunctionGroupState &other) = delete;
            FunctionGroupState(FunctionGroupState &&other);
            FunctionGroupState &operator=(const FunctionGroupState &other) = delete;
            FunctionGroupState &operator=(FunctionGroupState &&other);
            ~FunctionGroupState() noexcept = default;

            /// @brief Get the function group bound to the instance
            /// @returns Constant reference to the corresponding function group
            const FunctionGroup &GetFunctionGroup() const noexcept;

            /// @brief Get the state bound to the instance
            /// @returns Constant reference to the state meta-model
            const std::string &GetState() const noexcept;

            /// @brief Serialized the object
            /// @param[out] serializedObject Serialized object byte vector
            /// @note This is not an ARA specified method.
            void Serialize(std::vector<uint8_t> &serializedObject) const;

            inline bool operator==(const FunctionGroupState &other) const noexcept
            {
                return (mFunctionGroup == other.mFunctionGroup) &&
                       (mStateMetaModel == other.mStateMetaModel);
            }

            inline bool operator!=(const FunctionGroupState &other) const noexcept
            {
                return (mFunctionGroup != other.mFunctionGroup) ||
                       (mStateMetaModel != other.mStateMetaModel);
            }
        };
    }
}

#endif