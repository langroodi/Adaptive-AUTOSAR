#ifndef ALIVE_CHECK_REQUEST_H
#define ALIVE_CHECK_REQUEST_H

#include "./message.h"

namespace DoipLib
{
    /// @brief DoIP tester alive check request
    class AliveCheckRequest : public Message
    {
    private:
        static const PayloadType cPayloadType{PayloadType::AliveCheckRequest};

    protected:
         void GetPayload(std::vector<uint8_t> &payload) const override;

         virtual bool TrySetPayload(const std::vector<uint8_t> &payload) override;
         
    public:
        AliveCheckRequest() noexcept;
        
        /// @brief Constructor
        /// @param protocolVersion DoIP ISO protocol version
        explicit AliveCheckRequest(uint8_t protocolVersion) noexcept;
    };
}

#endif