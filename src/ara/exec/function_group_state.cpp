#include "./function_group_state.h"

namespace ara
{
    namespace exec
    {
        FunctionGroupState::FunctionGroupState(
            const FunctionGroup *functionGroup,
            std::string metaModelIdentifier) : mFunctionGroup{functionGroup},
                                               mStateMetaModel{metaModelIdentifier}
        {
        }

        core::Result<FunctionGroupState> FunctionGroupState::Create(
            const FunctionGroup &functionGroup,
            std::string metaModelIdentifier)
        {
            FunctionGroupState _functionGroupState{&functionGroup, metaModelIdentifier};
            core::Result<FunctionGroupState> _result{std::move(_functionGroupState)};

            return _result;
        }

        FunctionGroupState::FunctionGroupState(
            FunctionGroupState &&other) : mFunctionGroup{other.mFunctionGroup},
                                          mStateMetaModel{std::move(other.mStateMetaModel)}
        {
        }

        FunctionGroupState &FunctionGroupState::operator=(
            FunctionGroupState &&other)
        {
            mFunctionGroup = other.mFunctionGroup;
            mStateMetaModel = std::move(other.mStateMetaModel);

            return *this;
        }

        const FunctionGroup &FunctionGroupState::GetFunctionGroup() const noexcept
        {
            return *mFunctionGroup;
        }

        const std::string &FunctionGroupState::GetState() const noexcept
        {
            return mStateMetaModel;
        }
    }
}