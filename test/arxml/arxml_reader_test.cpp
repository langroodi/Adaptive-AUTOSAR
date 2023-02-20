#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <array>
#include "../../src/arxml/arxml_reader.h"

namespace arxml
{
    TEST(ArxmlReaderTest, ValidConstruction)
    {
        const std::string cContent =
            "<AR-PACKAGES><AR-PACKAGE><SHORT-NAME>pkg</SHORT-NAME></AR-PACKAGE></AR-PACKAGES>";

        EXPECT_NO_THROW(ArxmlReader _reader(cContent.c_str(), cContent.length()));
    }

    TEST(ArxmlReaderTest, ValidVersionConstruction)
    {
        const std::string cContent =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<AUTOSAR xmlns=\"http://autosar.org/schema/r4.0\"></AUTOSAR>";

        EXPECT_NO_THROW(ArxmlReader _reader(cContent.c_str(), cContent.length()));
    }

    TEST(ArxmlReaderTest, InvalidVersionConstruction)
    {
        const std::string cContent =
            "<?xml version=\"1.1\" encoding=\"UTF-8\"?>"
            "<AUTOSAR xmlns=\"http://autosar.org/schema/r4.0\"></AUTOSAR>";

        EXPECT_THROW(ArxmlReader _reader(cContent.c_str(), cContent.length()), std::invalid_argument);
    }

    TEST(ArxmlReaderTest, GetNodesMethod)
    {
        const std::string cPackage1{"pkg1"};
        const std::string cPackage2{"pkg2"};
        const std::array<std::string, 2> cExpectedResult{cPackage1, cPackage2};

        const std::string cContent =
            "<AR-PACKAGES>"
            "<AR-PACKAGE><SHORT-NAME>" + cPackage1 + "</SHORT-NAME></AR-PACKAGE>"
            "<AR-PACKAGE><SHORT-NAME>" + cPackage2 + "</SHORT-NAME></AR-PACKAGE>"
            "</AR-PACKAGES>";

        ArxmlReader _reader(cContent.c_str(), cContent.length());
        auto _nodes{_reader.GetNodes({"AR-PACKAGES"})};
        std::vector<std::string> _actualResult;

        for (auto node : _nodes)
        {
            _actualResult.emplace_back(node.GetShortName());
        }

        bool _areEqual{
            std::equal(
                _actualResult.cbegin(), _actualResult.cend(), cExpectedResult.cbegin())};
        EXPECT_TRUE(_areEqual);
    }
}