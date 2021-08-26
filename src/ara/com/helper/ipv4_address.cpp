#include "./ipv4_address.h"

namespace ara
{
    namespace com
    {
        namespace helper
        {
            void Ipv4Address::Inject(
                std::vector<uint8_t> &vector,
                Ipv4Address ipAddress)
            {
                vector.insert(
                    vector.end(),
                    ipAddress.Octets.begin(),
                    ipAddress.Octets.end());
            }
        };
    }
}