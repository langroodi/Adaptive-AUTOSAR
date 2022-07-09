#include <gtest/gtest.h>
#include "../../../src/ara/diag/security_access.h"

namespace ara
{
    namespace diag
    {
        TEST(SecurityAccessTest, HandleRequestSeedMessage)
        {
            const uint8_t cSid{0x27};
            const size_t cSidIndex{1};
            const uint8_t cNrc{0x12};
            const size_t cNrcIndex{2};
            const uint8_t cSubFunction{0x43};
            const ReentrancyType cReentrancy{ReentrancyType::kNot};

            core::InstanceSpecifier _specifier("Instance0");
            std::vector<uint8_t> _requestData{cSid, cSubFunction};
            MetaInfo _metaInfo(Context::kDoIP);
            CancellationHandler _cancellationHandler(false);
            SecurityAccess _securityAccess(_specifier, cReentrancy);

            std::future<OperationOutput> _responseFuture{
                _securityAccess.HandleMessage(
                    _requestData,
                    _metaInfo,
                    std::move(_cancellationHandler))};

            OperationOutput _response{_responseFuture.get()};

            uint8_t _sid{_response.responseData.at(cSidIndex)};
            EXPECT_EQ(cSid, _sid);

            uint8_t _nrc{_response.responseData.at(cNrcIndex)};
            EXPECT_EQ(cNrc, _nrc);
        }

        TEST(SecurityAccessTest, GetSeedMethod)
        {
            const size_t cSeedOffset{0};
            const uint8_t cSubFunction{0x01};
            const ReentrancyType cReentrancy{ReentrancyType::kNot};

            core::InstanceSpecifier _specifier("Instance0");
            std::vector<uint8_t> _securityAccessDataRecord;
            MetaInfo _metaInfo(Context::kDoIP);
            CancellationHandler _cancellationHandler(false);
            SecurityAccess _securityAccess(_specifier, cReentrancy);

            std::future<std::vector<uint8_t>> _seedByteArrFuture{
                _securityAccess.GetSeed(
                    cSubFunction,
                    _securityAccessDataRecord,
                    _metaInfo,
                    std::move(_cancellationHandler))};

            std::vector<uint8_t> _seedByteArr{_seedByteArrFuture.get()};
            size_t _offset{cSeedOffset};
            auto _expectedResult{static_cast<uint16_t>(_seedByteArr.at(_offset)) << 8};
            ++_offset;
            _expectedResult |= _seedByteArr.at(_offset);

            EXPECT_GT(_expectedResult, 0);

            CancellationHandler _newCancellationHandler(false);
            _seedByteArrFuture =
                _securityAccess.GetSeed(
                    cSubFunction,
                    _securityAccessDataRecord,
                    _metaInfo,
                    std::move(_newCancellationHandler));

            _seedByteArr = _seedByteArrFuture.get();
            _offset = cSeedOffset;
            auto _actualResult{static_cast<uint16_t>(_seedByteArr.at(_offset)) << 8};
            ++_offset;
            _actualResult |= _seedByteArr.at(_offset);

            EXPECT_EQ(_expectedResult, _actualResult);
        }
    }
}