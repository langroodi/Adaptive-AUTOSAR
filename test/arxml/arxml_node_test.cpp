#include <gtest/gtest.h>
#include "../../src/arxml/arxml_reader.h"

namespace arxml
{
    TEST(ArxmlNodeTest, GetShortNameMethod)
    {
        const std::string cExpectedResult{"pkg"};
        const std::string cContent =
            "<AR-PACKAGES><AR-PACKAGE>"
            "<SHORT-NAME>" +
            cExpectedResult +
            "</SHORT-NAME></AR-PACKAGE></AR-PACKAGES>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"AR-PACKAGES", "AR-PACKAGE"})};
        std::string _actualResult{_node.GetShortName()};

        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(ArxmlNodeTest, GetByteValueMethod)
    {
        const uint8_t cExpectedResult{1};
        const std::string cContent =
            "<COMMUNICATION-CLUSTER><ETHERNET-PHYSICAL-CHANNEL>"
            "<SHORT-NAME>Localhost</SHORT-NAME></ETHERNET-PHYSICAL-CHANNEL>"
            "<PROTOCOL-VERSION>" +
            std::to_string(cExpectedResult) +
            "</PROTOCOL-VERSION></COMMUNICATION-CLUSTER>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"COMMUNICATION-CLUSTER", "PROTOCOL-VERSION"})};
        auto _actualResult{_node.GetValue<uint8_t>()};

        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(ArxmlNodeTest, GetStringValueMethod)
    {
        const std::string cExpectedResult{"127.0.0.1"};
        const std::string cContent =
            "<NETWORK-ENDPOINT><SHORT-NAME>RpcServerEP</SHORT-NAME>"
            "<NETWORK-ENDPOINT-ADDRESSES><IPV-4-CONFIGURATION>"
            "<IPV-4-ADDRESS>" +
            cExpectedResult +
            "</IPV-4-ADDRESS></IPV-4-CONFIGURATION></NETWORK-ENDPOINT-ADDRESSES></NETWORK-ENDPOINT>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"NETWORK-ENDPOINT", "NETWORK-ENDPOINT-ADDRESSES", "IPV-4-CONFIGURATION", "IPV-4-ADDRESS"})};
        auto _actualResult{_node.GetValue<std::string>()};

        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(ArxmlNodeTest, ValidReferenceNodeTest)
    {
        const std::string cExpectedResult{"Signal1"};
        const std::string cSourceNode{"SYSTEM-SIGNAL-REF"};
        const std::string cDestinationType{"SYSTEM-SIGNAL"};
        const std::string cContent =
            "<AR-PACKAGES><AR-PACKAGE><SYSTEM-SIGNALS><" +
            cSourceNode + " DEST=\"" + cDestinationType + "\">" + cExpectedResult + "</" + cSourceNode + "><" +
            cDestinationType + "><SHORT-NAME>" + cExpectedResult + "</SHORT-NAME></" + cDestinationType +
            "></SYSTEM-SIGNALS></AR-PACKAGE></AR-PACKAGES>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"AR-PACKAGES", "AR-PACKAGE", "SYSTEM-SIGNALS"})};

        std::string _actualResult;
        EXPECT_TRUE(_node.TryGetReference(cSourceNode, cDestinationType, _actualResult));

        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(ArxmlNodeTest, InvalidReferenceNodeTest)
    {
        const std::string cSourceNode{"SYSTEM-SIGNAL-REF"};
        const std::string cDestinationType{"MODE-DECLARATION"};
        const std::string cContent =
            "<AR-PACKAGES><AR-PACKAGE><SYSTEM-SIGNALS><" +
            cSourceNode + " DEST=\"SYSTEM-SIGNAL\">Signal1</" + cSourceNode +
            "><SYSTEM-SIGNAL><SHORT-NAME>Signal1</SHORT-NAME></SYSTEM-SIGNAL>"
            "</SYSTEM-SIGNALS></AR-PACKAGE></AR-PACKAGES>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"AR-PACKAGES", "AR-PACKAGE", "SYSTEM-SIGNALS"})};

        std::string _actualResult;
        EXPECT_FALSE(_node.TryGetReference(cSourceNode, cDestinationType, _actualResult));
    }

    TEST(ArxmlNodeTest, GetContentMethod)
    {
        const std::string cExpectedResult{
            "<AR-PACKAGE>\n\t<SHORT-NAME>pkg</SHORT-NAME>\n</AR-PACKAGE>\n"};
        const std::string cContent =
            "<AR-PACKAGES>" + cExpectedResult + "</AR-PACKAGES>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"AR-PACKAGES", "AR-PACKAGE"})};
        std::string _actualResult{_node.GetContent()};

        EXPECT_EQ(cExpectedResult, _actualResult);
    }
}