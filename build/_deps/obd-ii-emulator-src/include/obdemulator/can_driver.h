#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include "./can_frame.h"

namespace ObdEmulator
{
    /// @brief CAN bus baud rate
    enum class CanBusSpeed : uint8_t
    {
        Speed5kbps = 0x0c,
        Speed10kbps = 0x0b,
        Speed20kbps = 0x0a,
        Speed50kbps = 0x09,
        Speed100kbps = 0x08,
        Speed125kbps = 0x07,
        Speed200kbps = 0x06,
        Speed250kbps = 0x05,
        Speed400kbps = 0x04,
        Speed500kbps = 0x03,
        Speed800kbps = 0x02,
        Speed1000kbps = 0x01
    };

    /// @brief CAN bus controller driver helper
    class CanDriver
    {
    public:
        /// @brief Fixed CAN frame data packet size
        static constexpr size_t cFixedFrameSize{20};

    private:
        static const size_t cStandardIdSize{2};
        static const size_t cExtendedIdSize{4};
        static const uint8_t cHeaderByte{0xaa};
        static const uint8_t cTrailerByte{0x55};
        static const uint8_t cExtendedBitMask{0x20};
        static const uint8_t cRemoteBitMask{0x10};

        const bool mSupportExtended;
        std::array<uint8_t, cFixedFrameSize> mConfiguration;

        static std::array<uint8_t, cStandardIdSize> getStandardIdArray(
            uint32_t id);
        static std::array<uint8_t, cExtendedIdSize> getExtendedIdArray(
            uint32_t id);

        static uint32_t getStandardId(
            uint8_t firstByte,
            uint8_t lastByte);

        static uint32_t getExtendedId(
            uint8_t firstByte,
            uint8_t secondtByte,
            uint8_t thirdByte,
            uint8_t lastByte);

        static void setChecksum(
            std::array<uint8_t, cFixedFrameSize> &data);

        static std::array<uint8_t, cFixedFrameSize> getConfigurationArray(
            CanBusSpeed speed, bool supportExtended);

    public:
        /// @brief Constructor
        /// @param speed CAN bus communication speed
        /// @param supportExtended Indicates whether extended frame is supported or not
        CanDriver(CanBusSpeed speed, bool supportExtended);
        CanDriver() = delete;

        /// @brief Get CAN bus communication configuration packet
        /// @returns Configuration packet byte vector
        std::vector<uint8_t> GetConfiguration() const;

        /// @brief Serialize a CAN frame to a variable length byte array
        /// @param frame Frame to be serialized
        /// @returns Serialized frame byte array
        /// @throws std::invalid_argument Throws if the input frame is invalid
        std::vector<uint8_t> Serialize(const CanFrame &frame) const;

        /// @brief Deserialize a packet to a CAN frame
        /// @param packet Packet data array to be deserialized
        /// @returns Derserialized CAN frame
        /// @throws std::invalid_argument Throws if the input packet is invalid
        CanFrame Deserialize(const std::vector<uint8_t> &packet) const;
    };
}

#endif