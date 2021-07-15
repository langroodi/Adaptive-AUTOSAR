#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <stdint.h>
#include <functional>
#include <vector>
#include <string_view>
#include "../core/result.h"
#include "./meta_info.h"

namespace ara
{
    namespace diag
    {
        enum class ActivityStatusType : std::uint8_t
        {
            kActive = 0x00,
            kInactive = 0x01
        };

        enum class SessionControlType : std::uint8_t
        {
            kDefaultSession = 0x01,
            kProgrammingSession = 0x02,
            kExtendedDiagnosticSession = 0x03,
            kSafetySystemDiagnosticSession = 0x04
        };

        enum class SecurityLevelType : std::uint8_t
        {
            kLocked = 0x00
        };

        struct ConversationIdentifierType
        {
            /* data */
        };

        class Conversation
        {
        public:
            ara::core::Result<ActivityStatusType> GetActivityStatus();
            ara::core::Result<void> SetActivityNotifier(
                std::function<void(ActivityStatusType)> notifier);
            ara::core::Result<ConversationIdentifierType> GetConversationIdentifier();
            ara::core::Result<SessionControlType> GetDiagnosticSession();
            ara::core::Result<void> SetDiagnosticSessionNotifier(
                std::function<void(SessionControlType)> notifier);
            ara::core::Result<std::string_view> GetDiagnosticSessionShortName(SessionControlType session);
            ara::core::Result<SecurityLevelType> GetDiagnosticSecurityLevel();
            ara::core::Result<void> SetSecurityLevelNotifier(
                std::function<void(SecurityLevelType)> notifier);
            ara::core::Result<std::string_view> GetDiagnosticSecurityLevelShortName(SecurityLevelType securityLevel);
            ara::core::Result<void> ResetToDefaultSession();

            static ara::core::Result<Conversation &> GetConversation(MetaInfo &metaInfo);
            static std::vector<Conversation &> GetAllConversations();
            static std::vector<Conversation &> GetCurrentActiveConversations();
        };
    }
}
#endif