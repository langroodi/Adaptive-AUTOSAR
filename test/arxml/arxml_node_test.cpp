#include <gtest/gtest.h>
#include "../../src/arxml/arxml_reader.h"

namespace arxml
{
    TEST(ArmxlNodeTest, GetShortNameMethod)
    {
        const std::string cExpectedResult{"pkg"};
        const std::string cContent =
            "<AR-PACKAGES><AR-PACKAGE>"
            "<SHORT-NAME>" + cExpectedResult + "</SHORT-NAME>"
            "</AR-PACKAGE></AR-PACKAGES>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"AR-PACKAGES", "AR-PACKAGE"})};
        std::string _actualResult{_node.GetShortName()};

        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(ArmxlNodeTest, ValidReferenceNodeTest)
    {
        const std::string cExpectedResult{"Signal1"};
        const std::string cSourceNode{"SYSTEM-SIGNAL-REF"};
        const std::string cDestinationType{"SYSTEM-SIGNAL"};
        const std::string cContent =
            "<AR-PACKAGES><AR-PACKAGE><SYSTEM-SIGNALS>"
            "<" + cSourceNode + " DEST=\""+ cDestinationType +"\">" + cExpectedResult + "</" + cSourceNode + ">"
            "<" + cDestinationType + "><SHORT-NAME>" + cExpectedResult + "</SHORT-NAME></" + cDestinationType + ">"
            "</SYSTEM-SIGNALS></AR-PACKAGE></AR-PACKAGES>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"AR-PACKAGES", "AR-PACKAGE", "SYSTEM-SIGNALS"})};

        std::string _actualResult;
        EXPECT_TRUE(_node.TryGetReference(cSourceNode, cDestinationType, _actualResult));

        EXPECT_EQ(cExpectedResult, _actualResult);
    }

    TEST(ArmxlNodeTest, InvalidReferenceNodeTest)
    {
        const std::string cSourceNode{"SYSTEM-SIGNAL-REF"};
        const std::string cDestinationType{"MODE-DECLARATION"};
        const std::string cContent =
            "<AR-PACKAGES><AR-PACKAGE><SYSTEM-SIGNALS>"
            "<" + cSourceNode + " DEST=\"SYSTEM-SIGNAL\">Signal1</" + cSourceNode + ">"
            "<SYSTEM-SIGNAL><SHORT-NAME>Signal1</SHORT-NAME></SYSTEM-SIGNAL>"
            "</SYSTEM-SIGNALS></AR-PACKAGE></AR-PACKAGES>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        ArxmlNode _node{
            _reader.GetRootNode(
                {"AR-PACKAGES", "AR-PACKAGE", "SYSTEM-SIGNALS"})};

        std::string _actualResult;
        EXPECT_FALSE(_node.TryGetReference(cSourceNode, cDestinationType, _actualResult));
    }
}