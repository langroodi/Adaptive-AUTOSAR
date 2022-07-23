#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <stdint.h>
#include <array>
#include <vector>

namespace ObdEmulator
{
    /// @brief CAN transmission packet frame
    class CanFrame
    {
    public:
        /// @brief Maximum CAN frame data length
        static const size_t cDataLengthMax{8};

    private:
        static const uint32_t cStandardIdMax{0x7ff};
        static const uint32_t cEXtendedIdMax{0x1fffffff};

        uint32_t mId;
        bool mExtended;
        bool mRemote;
        size_t mDataLength;
        std::array<uint8_t, cDataLengthMax> mData;

    public:
        CanFrame() = delete;

        /// @brief Constructor
        /// @param id CAN frame ID
        /// @param extended Indicates whether the frame is extended or standard
        /// @param remote Indicates whether the frame is remote transmission request (RTR) or data
        /// @param data CAN packet containing data
        /// @throws std::out_of_range Throws if the ID is out of range
        CanFrame(
            uint32_t id,
            bool extended,
            bool remote,
            const std::vector<uint8_t> &data);

        /// @brief Get frame CAN ID
        /// @returns 11-bit ID if the frame is standard and 20-bit ID if it is extended
        /// @see IsExtended
        u_int32_t GetId() const noexcept;

        /// @brief Indicate whether the frame is extended or standard
        /// @returns True if the frame is extended, otherwise false
        bool IsExtended() const noexcept;

        /// @brief Indicate whether the frame is remote transmission request (RTR) or data
        /// @returns True if the frame is RTR, otherwise false
        bool IsRemote() const noexcept;

        /// @brief Get frame data length (DLC)
        /// @returns A number between 0 to 8
        size_t GetDataLength() const noexcept;

        /// @brief Get frame data
        /// @returns Const reference to the data byte array
        const std::array<uint8_t, cDataLengthMax> &GetData() const noexcept;
    };
}

#endif