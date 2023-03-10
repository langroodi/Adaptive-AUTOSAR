#include <algorithm>
#include "./conversation.h"

namespace ara
{
    namespace diag
    {
        std::vector<Conversation> Conversation::mConversations;

        Conversation::Conversation(const MetaInfo &metaInfo) : mMetaInfo{metaInfo},
                                                               mActivityStatus{ActivityStatusType::kActive},
                                                               mDiagnosticSession{SessionControlType::kDefaultSession},
                                                               mDiagnosticSecurityLevel{SecurityLevelType::kLocked}
        {
            mConversationIdentifier.id = -1;
        }

        ara::core::Result<std::reference_wrapper<Conversation>> Conversation::GetConversation(MetaInfo &metaInfo)
        {
            Conversation _conversation(metaInfo);
            mConversations.push_back(std::move(_conversation));
            std::reference_wrapper<Conversation> _conversationRef(mConversations.back());
            ara::core::Result<std::reference_wrapper<Conversation>> _result(_conversationRef);

            return _result;
        }

        std::vector<std::reference_wrapper<Conversation>> Conversation::GetAllConversations()
        {
            std::vector<std::reference_wrapper<Conversation>> _result(
                mConversations.begin(), mConversations.end());

            return _result;
        }

        std::vector<std::reference_wrapper<Conversation>> Conversation::GetCurrentActiveConversations()
        {
            std::vector<std::reference_wrapper<Conversation>> _result;

            for (auto conversation : mConversations)
            {
                if (conversation.GetActivityStatus() == ActivityStatusType::kActive)
                {
                    _result.emplace_back(conversation);
                }
            }

            return _result;
        }

        ara::core::Result<ActivityStatusType> Conversation::GetActivityStatus()
        {
            ara::core::Result<ActivityStatusType> _result(mActivityStatus);
            return _result;
        }

        void Conversation::Deactivate()
        {
            if (mActivityStatus != ActivityStatusType::kInactive)
            {
                mActivityStatus = ActivityStatusType::kInactive;

                if (mActivityNotifier)
                {
                    mActivityNotifier(mActivityStatus);
                }
            }
        }

        ara::core::Result<void> Conversation::SetActivityNotifier(
            std::function<void(ActivityStatusType)> notifier)
        {
            mActivityNotifier = notifier;
            ara::core::Result<void> _result;

            return _result;
        }

        ara::core::Result<ConversationIdentifierType> Conversation::GetConversationIdentifier()
        {
            ara::core::Result<ConversationIdentifierType> _result(mConversationIdentifier);
            return _result;
        }

        ara::core::Result<SessionControlType> Conversation::GetDiagnosticSession()
        {
            ara::core::Result<SessionControlType> _result(mDiagnosticSession);
            return _result;
        }

        void Conversation::SetDiagnosticSession(SessionControlType diagnosticSession)
        {
            if (mDiagnosticSession != diagnosticSession)
            {
                mDiagnosticSession = diagnosticSession;
                if (mDiagnosticSessionNotifier)
                {
                    mDiagnosticSessionNotifier(mDiagnosticSession);
                }
            }
        }

        ara::core::Result<void> Conversation::SetDiagnosticSessionNotifier(
            std::function<void(SessionControlType)> notifier)
        {
            mDiagnosticSessionNotifier = notifier;
            ara::core::Result<void> _result;

            return _result;
        }

        ara::core::Result<std::string> Conversation::GetDiagnosticSessionShortName(
            SessionControlType session)
        {
            std::string _shortName;

            switch (session)
            {
            case SessionControlType::kDefaultSession:
                _shortName = "Default session";
                break;
            case SessionControlType::kProgrammingSession:
                _shortName = "Programming session";
                break;
            case SessionControlType::kExtendedDiagnosticSession:
                _shortName = "Extended diagnostic session";
                break;
            case SessionControlType::kSafetySystemDiagnosticSession:
                _shortName = "Safety system diagnostic session";
                break;
            default:
                _shortName = "Not supported";
            }

            ara::core::Result<std::string> _result(_shortName);

            return _result;
        }

        ara::core::Result<SecurityLevelType> Conversation::GetDiagnosticSecurityLevel()
        {
            ara::core::Result<SecurityLevelType> _result(mDiagnosticSecurityLevel);
            return _result;
        }

        ara::core::Result<void> Conversation::SetSecurityLevelNotifier(
            std::function<void(SecurityLevelType)> notifier)
        {
            mSecurityLevelNotifier = notifier;
            ara::core::Result<void> _result;

            return _result;
        }

        /// @brief Convert a given security level to a short name
        /// @param securityLevel Security level to be converted
        /// @returns Given security level short name
        ara::core::Result<std::string> Conversation::GetDiagnosticSecurityLevelShortName(
            SecurityLevelType securityLevel)
        {
            std::string _shortName;

            switch (securityLevel)
            {
            case SecurityLevelType::kLocked:
                _shortName = "Security level locked";
                break;
            default:
                _shortName = "Not supported";
            }

            ara::core::Result<std::string> _result(_shortName);

            return _result;
        }

        ara::core::Result<void> Conversation::ResetToDefaultSession()
        {
            SetDiagnosticSession(SessionControlType::kDefaultSession);

            if (mDiagnosticSecurityLevel != SecurityLevelType::kLocked)
            {
                mDiagnosticSecurityLevel = SecurityLevelType::kLocked;
                if (mSecurityLevelNotifier)
                {
                    mSecurityLevelNotifier(mDiagnosticSecurityLevel);
                }
            }

            ara::core::Result<void> _result;

            return _result;
        }
    }
}