#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include "../../../../../src/ara/com/someip/sd/someip_sd_message.h"
#include "../../../../../src/ara/com/entry/service_entry.h"
#include "../../../../../src/ara/com/option/loadbalancing_option.h"
#include "../../../../../src/ara/com/option/ipv4_endpoint_option.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                TEST(SomeIpSdMessageTest, Constructor)
                {
                    const uint16_t cSessionId = 1;
                    const SomeIpMessageType cMessageType = SomeIpMessageType::Notification;
                    const SomeIpReturnCode cReturnCode = SomeIpReturnCode::eOK;

                    SomeIpSdMessage _message;

                    EXPECT_EQ(_message.SessionId(), cSessionId);
                    EXPECT_EQ(_message.MessageType(), cMessageType);
                    EXPECT_EQ(_message.ReturnCode(), cReturnCode);
                }

                TEST(SomeIpSdMessageTest, SetInvalidSessionId)
                {
                    const uint16_t cInvalidSessionId = 0;

                    SomeIpSdMessage _message;

                    EXPECT_THROW(
                        _message.SetSessionId(cInvalidSessionId),
                        std::invalid_argument);
                }

                TEST(SomeIpSdMessageTest, SessionIdWrapping)
                {
                    const uint16_t cOldSessionId = 0xFFFF;
                    const uint16_t cNewSessionId = 1;

                    SomeIpSdMessage _message;
                    _message.SetSessionId(cOldSessionId);
                    bool _wrapped = _message.IncrementSessionId();

                    EXPECT_TRUE(_wrapped);
                    EXPECT_EQ(_message.SessionId(), cNewSessionId);
                }

                TEST(SomeIpSdMessageTest, PayloadMethod)
                {
                    const uint16_t cServiceId = 0x0001;
                    const uint16_t cInstanceId = 0x0002;
                    const uint8_t cMajorVersion = 0x03;
                    const uint32_t cMinorVersion = 0x00000004;

                    const bool cFirstDiscardable = true;
                    const uint16_t cPriority = 0;
                    const uint16_t cWeight = 7;

                    const bool cSecondDiscardable = false;
                    const helper::Ipv4Address cIpAddress(127, 0, 0, 1);
                    const option::Layer4ProtocolType cProtocol = option::Layer4ProtocolType::Tcp;
                    const uint16_t cPort = 8080;

                    auto _entry =
                        entry::ServiceEntry::CreateOfferServiceEntry(
                            cServiceId, cInstanceId, cMajorVersion, cMinorVersion);

                    std::unique_ptr<option::LoadBalancingOption> _firstOption =
                        std::make_unique<option::LoadBalancingOption>(
                            cFirstDiscardable, cPriority, cWeight);
                    _entry->AddFirstOption(std::move(_firstOption));

                    auto _secondOption =
                        option::Ipv4EndpointOption::CreateUnitcastEndpoint(
                            cSecondDiscardable, cIpAddress, cProtocol, cPort);
                    _entry->AddSecondOption(std::move(_secondOption));

                    SomeIpSdMessage _message;
                    _message.AddEntry(std::move(_entry));

                    const size_t cPayloadSize = 64;
                    const std::array<uint8_t, cPayloadSize> cExpectedPayload =
                        // SOME/IP Header:
                        {0xff, 0xff, 0x81, 0x00,
                         0x00, 0x00, 0x00, 0x38,
                         0x00, 0x00, 0x00, 0x01,
                         0x01, 0x01, 0x02, 0x00,
                         // Flags + reserved:
                         0xc0, 0x00, 0x00, 0x00,
                         // Entries length:
                         0x00, 0x00, 0x00, 0x10,
                         // Entry:
                         0x01, 0x00, 0x01, 0x11,
                         0x00, 0x01, 0x00, 0x02,
                         0x03, 0xff, 0xff, 0xff,
                         0x00, 0x00, 0x00, 0x04,
                         // Options length:
                         0x00, 0x00, 0x00, 0x14,
                         // First option:
                         0x00, 0x05, 0x02, 0x01,
                         0x00, 0x00, 0x00, 0x07,
                         // Second option:
                         0x00, 0x09, 0x04, 0x00,
                         0x7f, 0x00, 0x00, 0x01,
                         0x00, 0x06, 0x1f, 0x90};

                    auto _actualPayload = _message.Payload();

                    bool _areEqual =
                        std::equal(
                            _actualPayload.begin(),
                            _actualPayload.end(),
                            cExpectedPayload.begin());

                    EXPECT_TRUE(_areEqual);
                }

                TEST(SomeIpSdMessageTest, NoEntryDeserialization)
                {
                    SomeIpSdMessage _originalMessage;
                    auto _payload = _originalMessage.Payload();
                    auto _deserializedMessage =
                        SomeIpSdMessage::Deserialize(_payload);

                    EXPECT_EQ(
                        _originalMessage.MessageId(),
                        _deserializedMessage.MessageId());

                    EXPECT_EQ(
                        _originalMessage.ClientId(),
                        _deserializedMessage.ClientId());

                    EXPECT_EQ(
                        _originalMessage.SessionId(),
                        _deserializedMessage.SessionId());

                    EXPECT_EQ(
                        _originalMessage.ProtocolVersion(),
                        _deserializedMessage.ProtocolVersion());

                    EXPECT_EQ(
                        _originalMessage.InterfaceVersion(),
                        _deserializedMessage.InterfaceVersion());

                    EXPECT_EQ(
                        _originalMessage.MessageType(),
                        _deserializedMessage.MessageType());

                    EXPECT_EQ(
                        _originalMessage.ReturnCode(),
                        _deserializedMessage.ReturnCode());
                }

                TEST(SomeIpSdMessageTest, RebootFlagDeserialization)
                {
                    SomeIpSdMessage _originalMessage;
                    auto _payload = _originalMessage.Payload();

                    const std::size_t cSomeIpSdPduOffset = 16;
                    const uint8_t cInvalidByte = 0xff;
                    // Violate the reboot flag
                    _payload.at(cSomeIpSdPduOffset) = cInvalidByte;

                    EXPECT_THROW(
                        SomeIpSdMessage::Deserialize(_payload), std::out_of_range);
                }

                TEST(SomeIpSdMessageTest, Deserializing)
                {
                    const uint16_t cServiceId = 0x0001;
                    const uint16_t cInstanceId = 0x0002;
                    const uint8_t cMajorVersion = 0x03;
                    const uint32_t cMinorVersion = 0x00000004;

                    const bool cFirstDiscardable = true;
                    const uint16_t cPriority = 0;
                    const uint16_t cWeight = 7;

                    const bool cSecondDiscardable = false;
                    const helper::Ipv4Address cIpAddress(127, 0, 0, 1);
                    const option::Layer4ProtocolType cProtocol = option::Layer4ProtocolType::Tcp;
                    const uint16_t cPort = 8080;

                    auto _entry =
                        entry::ServiceEntry::CreateOfferServiceEntry(
                            cServiceId, cInstanceId, cMajorVersion, cMinorVersion);

                    std::unique_ptr<option::LoadBalancingOption> _firstOption =
                        std::make_unique<option::LoadBalancingOption>(
                            cFirstDiscardable, cPriority, cWeight);
                    _entry->AddFirstOption(std::move(_firstOption));

                    auto _secondOption =
                        option::Ipv4EndpointOption::CreateUnitcastEndpoint(
                            cSecondDiscardable, cIpAddress, cProtocol, cPort);
                    _entry->AddSecondOption(std::move(_secondOption));

                    SomeIpSdMessage _originalMessage;
                    _originalMessage.AddEntry(std::move(_entry));

                    auto _originalMessagePayload = _originalMessage.Payload();

                    SomeIpSdMessage _deserializedMessage =
                    SomeIpSdMessage::Deserialize(_originalMessagePayload);

                    auto _deserializedMessagePayload = _deserializedMessage.Payload();

                    bool _areEqual =
                        std::equal(
                            _deserializedMessagePayload.begin(),
                            _deserializedMessagePayload.end(),
                            _originalMessagePayload.begin());

                    EXPECT_TRUE(_areEqual);
                }
            }
        }
    }
}