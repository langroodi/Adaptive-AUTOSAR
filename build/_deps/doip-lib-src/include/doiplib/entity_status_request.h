#ifndef ENTITY_STATUS_REQUEST_H
#define ENTITY_STATUS_REQUEST_H

#include "./message.h"

namespace DoipLib
{
    /// @brief Diagnostic entity status request
    class EntityStatusRequest : public Message
    {
    private:
        static const PayloadType cPayloadType{
            PayloadType::DoipEntityStatusRequest};

    protected:
         void GetPayload(std::vector<uint8_t> &payload) const override;

         virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;
         
    public:
        EntityStatusRequest() noexcept;
        
        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        explicit EntityStatusRequest(uint8_t protocolVersion) noexcept;
    };
}

#endif