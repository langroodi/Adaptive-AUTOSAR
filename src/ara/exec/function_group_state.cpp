#include <algorithm>
#include "../com/helper/payload_helper.h"
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

        void FunctionGroupState::Serialize(
            std::vector<uint8_t> &serializedObject) const
        {
            // According to AUTOSAR R22-11 SOME/IP Protocol Specification,
            // string fields should start with a field length (32-bit by default)
            // and then the serialized string.
            std::string _functionGroupStr{mFunctionGroup->GetInstance().ToString()};
            auto _functionGroupLength{
                static_cast<uint32_t>(_functionGroupStr.length())};
            com::helper::Inject(serializedObject, _functionGroupLength);
            std::copy(
                _functionGroupStr.cbegin(),
                _functionGroupStr.cend(),
                std::back_inserter(serializedObject));

            auto _stateLength{
                static_cast<uint32_t>(mStateMetaModel.length())};
            com::helper::Inject(serializedObject, _stateLength);
            std::copy(
                mStateMetaModel.cbegin(),
                mStateMetaModel.cend(),
                std::back_inserter(serializedObject));
        }
    }
}