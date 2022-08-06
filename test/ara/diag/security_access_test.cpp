#include "./routing/testable_uds_service.h"
#include "../../../src/ara/diag/security_access.h"

namespace ara
{
    namespace diag
    {
        class SecurityAccessTest : public routing::TestableUdsService
        {
        private:
            static core::InstanceSpecifier mSpecifier;
            static const ReentrancyType cReentrancy;

            const uint8_t cAttemptThreshold{1};
            const int cExceededAttemptDelay{3600};

            const std::string cEncryptorKey{"Encryptor"};
            const std::string cAttemptThresholdKey{"AttemptThreshold"};
            const std::string cExceededAttemptDelayKey{"ExceededAttemptDelay"};

        protected:
            const uint16_t cEncryptor{0x1234};

            SecurityAccess Service;

        public:
            SecurityAccessTest() : Service{mSpecifier, cReentrancy}
            {
                std::string _encryptorStr{std::to_string(cEncryptor)};
                GeneralMetaInfo.SetValue(cEncryptorKey, _encryptorStr);

                std::string _attemptThresholdStr{std::to_string(cAttemptThreshold)};
                GeneralMetaInfo.SetValue(cAttemptThresholdKey, _attemptThresholdStr);

                std::string _exceededAttemptDelayStr{std::to_string(cExceededAttemptDelay)};
                GeneralMetaInfo.SetValue(cExceededAttemptDelayKey, _exceededAttemptDelayStr);
            }

            uint16_t GetSeed(uint8_t subFunction)
            {
                const size_t cSeedOffset{0};

                std::vector<uint8_t> _securityAccessDataRecord;
                CancellationHandler _cancellationHandler(false);

                std::future<std::vector<uint8_t>> _resultByteArrFuture{
                    Service.GetSeed(
                        subFunction,
                        _securityAccessDataRecord,
                        GeneralMetaInfo,
                        std::move(_cancellationHandler))};

                std::vector<uint8_t> _resultByteArr{_resultByteArrFuture.get()};
                size_t _offset{cSeedOffset};
                auto _result{static_cast<uint16_t>(_resultByteArr.at(_offset)) << 8};
                ++_offset;
                _result |= _resultByteArr.at(_offset);

                return _result;
            }
        };

        core::InstanceSpecifier SecurityAccessTest::mSpecifier{"Instance0"};
        const ReentrancyType SecurityAccessTest::cReentrancy{ReentrancyType::kNot};

        TEST_F(SecurityAccessTest, IncorrectRequestScenario)
        {
            const uint8_t cExpectedNrc{0x13};

            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{Service.GetSid()};
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(SecurityAccessTest, InvalidSubFunctionScenario)
        {
            const uint8_t cExpectedNrc{0x12};
            const uint8_t cSubFunction{0x43};

            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{Service.GetSid(), cSubFunction};
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(SecurityAccessTest, GetSeedMethod)
        {
            const uint8_t cSubFunction{0x01};

            uint16_t _expectedResult{GetSeed(cSubFunction)};
            EXPECT_GT(_expectedResult, 0);

            uint16_t _actualResult{GetSeed(cSubFunction)};
            EXPECT_EQ(_expectedResult, _actualResult);
        }

        TEST_F(SecurityAccessTest, IncorrectKeyLengthScenario)
        {
            const uint8_t cSubFunction{0x02};
            const uint8_t cExpectedNrc{0x13};

            uint8_t _actualNrc;
            std::vector<uint8_t> _requestData{Service.GetSid(), cSubFunction};
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(SecurityAccessTest, InvalidCompareKeyMethod)
        {
            const uint8_t cSubFunction{0x01};
            const uint8_t cKeyMsb{0x00};
            const uint8_t cKeyLsb{0x00};
            const KeyCompareResultType cExpectedResult{KeyCompareResultType::kKeyInvalid};

            GetSeed(cSubFunction);

            std::vector<uint8_t> _keyByteArr{cKeyMsb, cKeyMsb};
            CancellationHandler _cancellationHandler(false);

            std::future<KeyCompareResultType> _actualResultFuture{
                Service.CompareKey(
                    cSubFunction,
                    _keyByteArr,
                    GeneralMetaInfo,
                    std::move(_cancellationHandler))};

            KeyCompareResultType _actualResult{_actualResultFuture.get()};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST_F(SecurityAccessTest, ValidCompareKeyMethod)
        {
            const uint8_t cSubFunction{0x01};
            const KeyCompareResultType cExpectedResult{KeyCompareResultType::kKeyValid};

            uint16_t _seed{GetSeed(cSubFunction)};

            auto _key{static_cast<uint16_t>(_seed ^ cEncryptor)};
            auto _keyMsb{static_cast<uint8_t>(_key >> 8)};
            auto _keyLsb{static_cast<uint8_t>(_key)};
            std::vector<uint8_t> _keyByteArr{_keyMsb, _keyLsb};

            CancellationHandler _cancellationHandler(false);

            std::future<KeyCompareResultType> _actualResultFuture{
                Service.CompareKey(
                    cSubFunction,
                    _keyByteArr,
                    GeneralMetaInfo,
                    std::move(_cancellationHandler))};

            KeyCompareResultType _actualResult{_actualResultFuture.get()};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }

        TEST_F(SecurityAccessTest, SequenceErrorScenario)
        {
            const uint8_t cExpectedNrc{0x24};
            const uint8_t cSubFunction{0x02};
            const uint8_t cKeyMsb{0x00};
            const uint8_t cKeyLsb{0x00};

            std::vector<uint8_t> _requestData{Service.GetSid(), cSubFunction, cKeyMsb, cKeyLsb};
            uint8_t _actualNrc;
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(SecurityAccessTest, InvalidKeyScenrio)
        {
            const uint8_t cExpectedNrc{0x35};
            const uint8_t cSubFunction{0x01};
            const uint8_t cKeyMsb{0x00};
            const uint8_t cKeyLsb{0x00};

            GetSeed(cSubFunction);

            auto _keyCompareSubFunction{static_cast<uint8_t>(cSubFunction + 1)};
            std::vector<uint8_t> _requestData{Service.GetSid(), _keyCompareSubFunction, cKeyMsb, cKeyLsb};
            uint8_t _actualNrc;
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(SecurityAccessTest, ExceededNumberOfAttemptsScenario)
        {
            const uint8_t cExpectedNrc{0x36};
            const uint8_t cSubFunction{0x01};
            const uint8_t cKeyMsb{0x00};
            const uint8_t cKeyLsb{0x00};

            GetSeed(cSubFunction);

            auto _keyCompareSubFunction{static_cast<uint8_t>(cSubFunction + 1)};
            std::vector<uint8_t> _requestData{Service.GetSid(), _keyCompareSubFunction, cKeyMsb, cKeyLsb};
            uint8_t _actualNrc;
            TryGetNrc(&Service, _requestData, _actualNrc);
            bool _hasNrc{TryGetNrc(&Service, _requestData, _actualNrc)};

            EXPECT_TRUE(_hasNrc);
            EXPECT_EQ(cExpectedNrc, _actualNrc);
        }

        TEST_F(SecurityAccessTest, ValidKeyScenrio)
        {
            const uint8_t cSubFunction{0x01};
            const size_t cSidIndex{0};
            const size_t cSubFunctionIndex{1};
            const uint8_t cPositiveResponseSidIncrement{0x40};

            uint16_t _seed{GetSeed(cSubFunction)};
            auto _key{static_cast<uint16_t>(_seed ^ cEncryptor)};
            auto _keyMsb{static_cast<uint8_t>(_key >> 8)};
            auto _keyLsb{static_cast<uint8_t>(_key)};

            auto _keyCompareSubFunction{static_cast<uint8_t>(cSubFunction + 1)};
            std::vector<uint8_t> _requestData{Service.GetSid(), _keyCompareSubFunction, _keyMsb, _keyLsb};
            CancellationHandler _cancellationHandler(false);

            std::future<OperationOutput> _responseFuture{
                Service.HandleMessage(
                    _requestData, GeneralMetaInfo, std::move(_cancellationHandler))};

            OperationOutput _response{_responseFuture.get()};

            auto _expectedSid{static_cast<uint8_t>(Service.GetSid() + cPositiveResponseSidIncrement)};
            uint8_t _actualSid{_response.responseData.at(cSidIndex)};
            EXPECT_EQ(_expectedSid, _actualSid);

            uint8_t _subFunction{_response.responseData.at(cSubFunctionIndex)};
            EXPECT_EQ(_keyCompareSubFunction, _subFunction);
        }

        TEST_F(SecurityAccessTest, UnlockedLevelSeedScenario)
        {
            const uint8_t cSubFunction{0x01};
            const uint8_t cExpectedResult{0};

            uint16_t _seed{GetSeed(cSubFunction)};
            auto _key{static_cast<uint16_t>(_seed ^ cEncryptor)};
            auto _keyMsb{static_cast<uint8_t>(_key >> 8)};
            auto _keyLsb{static_cast<uint8_t>(_key)};

            auto _keyCompareSubFunction{static_cast<uint8_t>(cSubFunction + 1)};
            std::vector<uint8_t> _requestData{Service.GetSid(), _keyCompareSubFunction, _keyMsb, _keyLsb};
            CancellationHandler _cancellationHandler(false);

            Service.HandleMessage(_requestData, GeneralMetaInfo, std::move(_cancellationHandler));

            uint16_t _actualResult{GetSeed(cSubFunction)};

            EXPECT_EQ(cExpectedResult, _actualResult);
        }
    }
}