#ifndef ARXML_READER_H
#define ARXML_READER_H

#include <string>
#include <initializer_list>
#include "./arxml_node_range.h"

/// @brief AUTOSAR XML (ARXML) configuration files utilities namespace
namespace arxml
{
    /// @brief A class to read ARXML configuration files
    class ArxmlReader
    {
    private:
        pugi::xml_document mDocument;

        bool validate(const pugi::xml_parse_result &parseResult) const;

    public:
        /// @brief Constructor
        /// @param filePath ARXML document file path
        /// @throws std::invalid_argument Throws when the ARXML file is not valid
        explicit ArxmlReader(const std::string &filePath);

        /// @brief Constructor
        /// @param content ARXML document file content
        /// @param length Content length
        ArxmlReader(const char *content, std::size_t length);

        ArxmlReader() = delete;
        ~ArxmlReader() noexcept = default;

        /// @brief Get (relative) ARXML root node
        /// @param children Hierarchical children nodes
        /// @returns ARXML root node relative to the children nodes (if there is any)
        ArxmlNode GetRootNode(std::initializer_list<std::string> children) const;

        /// @brief Get a range of ARXML nodes deep down the ARXML content
        /// @param children Hierarchical children nodes
        /// @returns Read-only range of ARXML nodes
        ArxmlNodeRange GetNodes(std::initializer_list<std::string> children) const;
    };
}

#endif