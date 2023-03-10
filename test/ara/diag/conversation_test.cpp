#include <gtest/gtest.h>
#include "../../../src/ara/diag/conversation.h"

namespace ara
{
    namespace diag
    {
        class ConversationTest : public testing::Test
        {
        protected:
            MetaInfo DoipMetaInfo;
            std::reference_wrapper<Conversation> ConversationReference;
            SessionControlType ActiveSession;

        public:
            ConversationTest() : DoipMetaInfo(Context::kDoIP),
                                 ConversationReference{Conversation::GetConversation(DoipMetaInfo).Value()},
                                 ActiveSession{SessionControlType::kDefaultSession}
            {
            }

            void ActiveSessionChanged(SessionControlType activeSession)
            {
                ActiveSession = activeSession;
            }
        };

        TEST_F(ConversationTest, Factory)
        {
            const ActivityStatusType cExpectedActivityStatus{
                ActivityStatusType::kActive};
            ActivityStatusType _actualActivityStatus{
                ConversationReference.get().GetActivityStatus().Value()};
            EXPECT_EQ(cExpectedActivityStatus, _actualActivityStatus);

            const SessionControlType cExpectedDiagnosticSession{
                SessionControlType::kDefaultSession};
            SessionControlType _actualDiagnosticSession{
                ConversationReference.get().GetDiagnosticSession().Value()};
            EXPECT_EQ(cExpectedDiagnosticSession, _actualDiagnosticSession);

            const SecurityLevelType cExpectedSecuirtyLevel{
                SecurityLevelType::kLocked};
            SecurityLevelType _actualSecurityLevel{
                ConversationReference.get().GetDiagnosticSecurityLevel().Value()};
            EXPECT_EQ(cExpectedSecuirtyLevel, _actualSecurityLevel);
        }

        TEST_F(ConversationTest, GetAllConversationsMethod)
        {
            auto _converstation{Conversation::GetAllConversations()};

            const size_t cExpectedResult{1};
            size_t _actualResult{_converstation.size()};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST_F(ConversationTest, GetCurrentActiveConversationsMethod)
        {
            auto _activeConverstations{Conversation::GetCurrentActiveConversations()};

            const size_t cExpectedResult{1};
            size_t _actualResult{_activeConverstations.size()};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST_F(ConversationTest, DiagnosticSessionScenario)
        {
            const SessionControlType cDefaultSession{
                SessionControlType::kDefaultSession};
            const SessionControlType cNewSession{
                SessionControlType::kProgrammingSession};

            auto _callback{
                std::bind(
                    &ConversationTest::ActiveSessionChanged,
                    this, std::placeholders::_1)};

            ConversationReference.get().SetDiagnosticSessionNotifier(_callback);
            ConversationReference.get().SetDiagnosticSession(cNewSession);
            EXPECT_EQ(cNewSession, ActiveSession);

            ConversationReference.get().ResetToDefaultSession();
            EXPECT_EQ(cDefaultSession, ActiveSession);
        }

        TEST_F(ConversationTest, DeactivateMethod)
        {
            const ActivityStatusType cExpectedResult{
                ActivityStatusType::kInactive};

            ActivityStatusType _actualResult;

            auto _callback = [&](ActivityStatusType status)
            {
                _actualResult = status;
            };

            ConversationReference.get().SetActivityNotifier(_callback);

            ConversationReference.get().Deactivate();
            EXPECT_EQ(cExpectedResult, _actualResult);

            _actualResult = ConversationReference.get().GetActivityStatus().Value();
            EXPECT_EQ(cExpectedResult, _actualResult);

            auto _activeConverstations{Conversation::GetCurrentActiveConversations()};
            EXPECT_TRUE(_activeConverstations.empty());
        }
    }
}