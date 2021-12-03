#include "./payload_helper.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            void Inject(std::vector<uint8_t> &vector, uint16_t value)
            {
                uint8_t _byte;

                _byte = value >> 8;
                vector.push_back(_byte);

                _byte = value;
                vector.push_back(_byte);
            }

            void Inject(std::vector<uint8_t> &vector, uint32_t value)
            {
                uint8_t _byte;

                _byte = value >> 24;
                vector.push_back(_byte);

                _byte = value >> 16;
                vector.push_back(_byte);

                _byte = value >> 8;
                vector.push_back(_byte);

                _byte = value;
                vector.push_back(_byte);
            }

            void Concat(std::vector<uint8_t> &vector1, std::vector<uint8_t> &&vector2)
            {
                vector1.insert(vector1.end(), vector2.begin(), vector2.end());
            }

            uint16_t ExtractShort(
                const std::vector<uint8_t> &vector, std::size_t &offset)
            {
                uint16_t _result = vector.at(offset) << 8;
                ++offset;
                _result |= vector.at(offset);
                ++offset;

                return _result;
            }

            uint32_t ExtractInteger(
                const std::vector<uint8_t> &vector, std::size_t &offset)
            {
                uint32_t _result = vector.at(offset) << 24;
                ++offset;
                _result |= vector.at(offset) << 16;
                ++offset;
                _result |= vector.at(offset) << 8;
                ++offset;
                _result |= vector.at(offset);
                ++offset;

                return _result;
            }
        }
    }
}