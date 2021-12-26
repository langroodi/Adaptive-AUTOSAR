#include "./execution_client.h"

namespace ara
{
    namespace exec
    {

        ExecutionClient::ExecutionClient(
            core::InstanceSpecifier instanceSpecifier,
            helper::FifoLayer<FifoMessageType> *communicationLayer) : mInstanceSpecifier{instanceSpecifier},
                                                                      mCommunicationLayer{communicationLayer}
        {
        }

        ara::core::Result<void> ExecutionClient::ReportExecutionState(
            ExecutionState state) const
        {
            auto _message = std::make_pair(mInstanceSpecifier, state);
            mCommunicationLayer->Send(_message);

            core::Result<void> _result;
            return _result;
        }
    }
}