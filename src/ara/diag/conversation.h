#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <stdint.h>
#include <functional>
#include <vector>
#include <string>
#include "../core/result.h"
#include "./meta_info.h"

namespace ara
{
    namespace diag
    {
        /// @brief Conversation enable status
        enum class ActivityStatusType : uint8_t
        {
            kActive = 0x00,  ///< Active conversation due to a request processing or a non-default session
            kInactive = 0x01 ///< Inactive conversation
        };

        /// @brief Diagnostic session control type
        enum class SessionControlType : uint8_t
        {
            kDefaultSession = 0x01,               ///< Default diagnostic session
            kProgrammingSession = 0x02,           ///< Session to enable memory programming diagnostic services
            kExtendedDiagnosticSession = 0x03,    ///< Session to enable function adjustment diagnostic services
            kSafetySystemDiagnosticSession = 0x04 ///< Session to enable safety system diagnostic services
        };

        /// @brief UDS security level access status
        enum class SecurityLevelType : uint8_t
        {
            kLocked = 0x00 ///< Locked security level (no access granted)
        };

        /// @brief Type to identify a conversation
        struct ConversationIdentifierType
        {
            /// @brief Conversation ID
            int64_t id;
        };

        /// @brief A wrapper around the communication between a diagnostic client and a diagnostic server
        /// @note The class is not fully aligned with the ARA standard.
        class Conversation
        {
        private:
            static std::vector<Conversation> mConversations;

            const MetaInfo &mMetaInfo;
            ActivityStatusType mActivityStatus;
            std::function<void(ActivityStatusType)> mActivityNotifier;
            ConversationIdentifierType mConversationIdentifier;
            SessionControlType mDiagnosticSession;
            std::function<void(SessionControlType)> mDiagnosticSessionNotifier;
            SecurityLevelType mDiagnosticSecurityLevel;
            std::function<void(SecurityLevelType)> mSecurityLevelNotifier;

            Conversation(const MetaInfo &metaInfo);

        public:
            /// @brief Conversation factory
            /// @param metaInfo Additional information to create a conversation
            /// @returns Reference to the created conversation in case of a valid meta information
            /// @note Return type signature is not compatible with ARA standard.
            static ara::core::Result<std::reference_wrapper<Conversation>> GetConversation(MetaInfo &metaInfo);

            /// @brief Get all the existing conversations
            /// @returns Active and inactive conversations collection
            static std::vector<std::reference_wrapper<Conversation>> GetAllConversations();

            /// @brief Get all the active conversations
            /// @returns Active conversations collection
            static std::vector<std::reference_wrapper<Conversation>> GetCurrentActiveConversations();

            Conversation() = delete;

            /// @brief Activity status property getter
            /// @returns Active or inactive status
            ara::core::Result<ActivityStatusType> GetActivityStatus();

            /// @brief Set the conversation activity status to 'inactive'
            void Deactivate();

            /// @brief Register an activity status change callback
            /// @param notifier Callback to be invoked when the activity status changed
            /// @returns No error will be returned
            ara::core::Result<void> SetActivityNotifier(
                std::function<void(ActivityStatusType)> notifier);

            /// @brief Conversation ID property getter
            /// @returns Conversation ID
            ara::core::Result<ConversationIdentifierType> GetConversationIdentifier();

            /// @brief Diagnostic session property getter
            /// @returns Coversation active session
            ara::core::Result<SessionControlType> GetDiagnosticSession();

            /// @brief Diagnostic session property setter
            /// @param diagnosticSession New diagnostic session
            /// @note The method is not ARA specific.
            void SetDiagnosticSession(SessionControlType diagnosticSession);

            /// @brief Register a diagnostic session change callback
            /// @param notifier Callback to be invoked when the diagnostic session changed
            /// @returns No error will be returned
            ara::core::Result<void> SetDiagnosticSessionNotifier(
                std::function<void(SessionControlType)> notifier);

            /// @brief Convert a given session to a short name
            /// @param session Session to be converted
            /// @returns Given session short name
            ara::core::Result<std::string> GetDiagnosticSessionShortName(
                SessionControlType session);

            /// @brief Security level property getter
            /// @returns Status of the active security level
            ara::core::Result<SecurityLevelType> GetDiagnosticSecurityLevel();

            /// @brief Register a security level change callback
            /// @param notifier Callback to be invoked when the security level status changed
            /// @returns No error will be returned
            ara::core::Result<void> SetSecurityLevelNotifier(
                std::function<void(SecurityLevelType)> notifier);

            /// @brief Convert a given security level to a short name
            /// @param securityLevel Security level to be converted
            /// @returns Given security level short name
            ara::core::Result<std::string> GetDiagnosticSecurityLevelShortName(
                SecurityLevelType securityLevel);

            /// @brief Reset the ongoing session to default
            /// @returns No error in case of a successful reset
            ara::core::Result<void> ResetToDefaultSession();
        };
    }
}

#endif