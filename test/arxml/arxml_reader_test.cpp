#include <gtest/gtest.h>
#include "../../src/arxml/arxml_reader.h"

namespace arxml
{
    TEST(ArmxlReaderTest, ValidConstruction)
    {
        const std::string cContent =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<AUTOSAR xmlns=\"http://autosar.org/schema/r4.0\"></AUTOSAR>";

        EXPECT_NO_THROW(ArxmlReader _reader(cContent.c_str(), cContent.length()));
    }

    TEST(ArmxlReaderTest, InvalidVersionConstruction)
    {
        const std::string cContent =
            "<?xml version=\"1.1\" encoding=\"UTF-8\"?>"
            "<AUTOSAR xmlns=\"http://autosar.org/schema/r4.0\"></AUTOSAR>";

        EXPECT_THROW(ArxmlReader _reader(cContent.c_str(), cContent.length()), std::invalid_argument);
    }
}