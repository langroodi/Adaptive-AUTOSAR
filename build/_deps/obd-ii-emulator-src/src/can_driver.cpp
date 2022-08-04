#include <obdemulator/can_driver.h>
#include <numeric>
#include <iterator>
#include <stdexcept>

namespace ObdEmulator
{
    const uint8_t CanDriver::cHeaderByte;
    const uint8_t CanDriver::cTrailerByte;

    std::array<uint8_t, CanDriver::cStandardIdSize> CanDriver::getStandardIdArray(
        uint32_t id)
    {
        const uint32_t cByteMask{0x000000ff};
        uint8_t _firstByte{static_cast<uint8_t>(id & cByteMask)};

        uint32_t _lastByteInt{id >> 8};
        uint8_t _lastByte{static_cast<uint8_t>(_lastByteInt & cByteMask)};

        std::array<uint8_t, CanDriver::cStandardIdSize> _result{
            _firstByte, _lastByte};

        return _result;
    }

    std::array<uint8_t, CanDriver::cExtendedIdSize> CanDriver::getExtendedIdArray(
        uint32_t id)
    {
        const uint32_t cByteMask{0x000000ff};
        uint8_t _firstByte{static_cast<uint8_t>(id & cByteMask)};

        uint32_t _secondByteInt{id >> 8};
        uint8_t _secondByte{static_cast<uint8_t>(_secondByteInt & cByteMask)};

        uint32_t _thirdByteInt{id >> 16};
        uint8_t _thirdByte{static_cast<uint8_t>(_thirdByteInt & cByteMask)};

        uint32_t _lastByteInt{id >> 24};
        uint8_t _lastByte{static_cast<uint8_t>(_lastByteInt & cByteMask)};

        std::array<uint8_t, CanDriver::cExtendedIdSize> _result{
            _firstByte, _secondByte, _thirdByte, _lastByte};

        return _result;
    }

    uint32_t CanDriver::getStandardId(
        uint8_t firstByte,
        uint8_t lastByte)
    {
        uint32_t _result{lastByte};

        _result <<= 8;
        _result |= firstByte;

        return _result;
    }

    uint32_t CanDriver::getExtendedId(
        uint8_t firstByte,
        uint8_t secondtByte,
        uint8_t thirdByte,
        uint8_t lastByte)
    {
        uint32_t _result{lastByte};

        _result <<= 8;
        _result |= thirdByte;

        _result <<= 8;
        _result |= secondtByte;

        _result <<= 8;
        _result |= firstByte;

        return _result;
    }

    void CanDriver::setChecksum(
        std::array<uint8_t, CanDriver::cFixedFrameSize> &data)
    {
        const size_t cChecksumBeginOffset{2};
        const size_t cChecksumEndOffset{1};
        const uint16_t cInitialChecksum{0};

        uint16_t _sum{
            std::accumulate(
                data.begin() + cChecksumBeginOffset,
                data.end() - cChecksumEndOffset,
                cInitialChecksum)};

        uint8_t _checksum{static_cast<uint8_t>(_sum)};
        data[CanDriver::cFixedFrameSize - cChecksumEndOffset] = _checksum;
    }

    std::array<uint8_t, CanDriver::cFixedFrameSize> CanDriver::getConfigurationArray(
        CanBusSpeed speed, bool supportExtended)
    {
        const uint8_t cConfigByte{0x12};
        const uint8_t cModeByte{0x00};
        const uint8_t cReservedByte{0x01};
        const uint8_t cExtendedByte{0x02};
        const uint8_t cStandardByte{0x01};

        uint8_t _speedByte{static_cast<uint8_t>(speed)};
        uint8_t _frameByte{supportExtended ? cExtendedByte : cStandardByte};

        std::array<uint8_t, CanDriver::cFixedFrameSize> _result{
            cHeaderByte,
            cTrailerByte,
            cConfigByte,
            _speedByte,
            _frameByte,
            // No filtered ID
            0, 0, 0, 0,
            // No ID filtering mask
            0, 0, 0, 0,
            cModeByte,
            cReservedByte,
            0, 0, 0, 0};

        setChecksum(_result);

        return _result;
    }

    CanDriver::CanDriver(
        CanBusSpeed speed, bool supportExtended) : mSupportExtended{supportExtended}
    {
        mConfiguration = getConfigurationArray(speed, supportExtended);
    }

    std::vector<uint8_t> CanDriver::GetConfiguration() const
    {
        std::vector<uint8_t> _result(CanDriver::cFixedFrameSize);
        std::copy(mConfiguration.cbegin(), mConfiguration.cend(), _result.begin());

        return _result;
    }

    std::vector<uint8_t> CanDriver::Serialize(const CanFrame &frame) const
    {
        if (!mSupportExtended && frame.IsExtended())
        {
            throw std::invalid_argument("Extended CAN ID is not supported.");
        }

        std::vector<uint8_t> _result;
        // Packet header segment
        _result.push_back(cHeaderByte);

        // Packet type segment
        // [Bit7: 1 (reserved)]
        // [Bit6: 1 (reserved)]
        uint8_t _typeByte{0xc0};
        // [Bit5: ExtendedFrame ? 1 : 0]
        if (frame.IsExtended())
        {
            _typeByte |= cExtendedBitMask;
        }
        // [Bit4: RTR ? 1 : 0]
        if (frame.IsRemote())
        {
            _typeByte |= cRemoteBitMask;
        }
        size_t _dlc{frame.GetDataLength()};
        uint8_t _dlcByte{static_cast<uint8_t>(_dlc)};
        // [Bit3~0: DLC (0~8)]
        _typeByte |= _dlcByte;
        _result.push_back(_typeByte);

        // Packet ID segment
        if (frame.IsExtended())
        {
            std::array<uint8_t, CanDriver::cExtendedIdSize> _array{
                getExtendedIdArray(frame.GetId())};
            std::copy(
                _array.cbegin(),
                _array.cbegin() + CanDriver::cExtendedIdSize,
                std::back_inserter(_result));
        }
        else
        {
            std::array<uint8_t, CanDriver::cStandardIdSize> _array{
                getStandardIdArray(frame.GetId())};
            std::copy(
                _array.cbegin(),
                _array.cbegin() + CanDriver::cStandardIdSize,
                std::back_inserter(_result));
        }

        // Packet data segment
        auto _data{frame.GetData()};
        std::copy(_data.cbegin(), _data.cbegin() + _dlc, std::back_inserter(_result));

        // Packet trailer segment
        _result.push_back(cTrailerByte);

        return _result;
    }

    CanFrame CanDriver::Deserialize(const std::vector<uint8_t> &packet) const
    {
        const uint8_t cDlcMask{0x0F};
        int _pointer{-1};

        // Packet header segment
        for (int i = 0; i < packet.size(); ++i)
        {
            if (packet.at(i) == cHeaderByte)
            {
                _pointer = i;
                break;
            }
        }

        if (_pointer == -1)
        {
            throw std::invalid_argument("Data header is corrupted.");
        }

        // Packet type segment
        uint8_t _typeByte = packet.at(++_pointer);
        bool _extended{static_cast<bool>(_typeByte & cExtendedBitMask)};
        if (!mSupportExtended && _extended)
        {
            throw std::invalid_argument("Extended CAN ID is not supported.");
        }
        bool _remote{static_cast<bool>(_typeByte & cRemoteBitMask)};
        size_t _dlc{static_cast<size_t>(_typeByte & cDlcMask)};

        // Packet ID segment
        uint32_t _id;
        if (_extended)
        {
            uint8_t _firstByte{packet.at(++_pointer)};
            uint8_t _secondByte{packet.at(++_pointer)};
            uint8_t _thirdByte{packet.at(++_pointer)};
            uint8_t _lastByte{packet.at(++_pointer)};
            _id = getExtendedId(_firstByte, _secondByte, _thirdByte, _lastByte);
        }
        else
        {
            uint8_t _firstByte{packet.at(++_pointer)};
            uint8_t _lastByte{packet.at(++_pointer)};
            _id = getStandardId(_firstByte, _lastByte);
        }

        // Packet data segment
        ++_pointer;
        auto _begin{packet.cbegin() + _pointer};
        auto _end{_begin + _dlc};
        std::vector<uint8_t> _data(_begin, _end);

        // Trailer segment
        _pointer += _dlc;
        uint8_t _trailerByte = packet.at(_pointer);
        if (_trailerByte != cTrailerByte)
        {
            throw std::invalid_argument("Data trailer is corrupted.");
        }

        CanFrame _result(_id, _extended, _remote, _data);

        return _result;
    }
}