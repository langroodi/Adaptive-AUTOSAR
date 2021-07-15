#ifndef META_INFO_H
#define META_INFO_H

#include <stdint.h>
#include <string_view>

namespace ara
{
    namespace diag
    {
        enum class Context : std::int32_t
        {
            kDiagnosticCommunication,
            kFaultMemory,
            kDoIP
        };

        class MetaInfo final
        {
        public:
            MetaInfo() noexcept = delete;
            MetaInfo(const MetaInfo &) = delete;
            MetaInfo(MetaInfo &&) noexcept = default;
            MetaInfo &operator=(const MetaInfo &) = delete;
            MetaInfo &operator=(MetaInfo &&) &noexcept = default;
            Context GetValue(std::string_view key) const noexcept;
            Context GetContext() const noexcept;
            ~MetaInfo() = default;
        };
    }
}

#endif