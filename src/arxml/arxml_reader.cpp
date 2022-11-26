#include <stdexcept>
#include <utility>
#include "./arxml_reader.h"

namespace arxml
{
    ArxmlReader::ArxmlReader(const std::string &filePath)
    {
        pugi::xml_parse_result _parseResult{
            mDocument.load_file(filePath.c_str(), pugi::parse_declaration)};

        if (!validate(_parseResult))
        {
            throw std::invalid_argument("Invalid ARXML document file!");
        }
    }

    ArxmlReader::ArxmlReader(const char *content, std::size_t length)
    {
        pugi::xml_parse_result _parseResult{
            mDocument.load_buffer(content, length, pugi::parse_declaration)};

        if (!validate(_parseResult))
        {
            throw std::invalid_argument("Invalid ARXML file content!");
        }
    }

    bool ArxmlReader::validate(const pugi::xml_parse_result &parseResult) const
    {
        // According to the standard, ARXML is only supported at XML v1.0 and UTF-8 encoding.
        const pugi::xml_encoding cValidEncoding{pugi::xml_encoding::encoding_utf8};
        const char *cXmlChild{"xml"};
        const char *cVersionAttrib{"version"};
        const float cValidVersion{1.0f};

        bool _result;

        if (parseResult.status == pugi::xml_parse_status::status_ok &&
            parseResult.encoding == cValidEncoding)
        {
            pugi::xml_node _declarationNode{mDocument.child(cXmlChild)};

            // Check the XML version if the document root node is a declaration node
            if (_declarationNode.type() == pugi::xml_node_type::node_declaration)
            {
                float _version{_declarationNode.attribute(cVersionAttrib).as_float()};
                _result = _version == cValidVersion;
            }
            else
            {
                _result = true;
            }
        }
        else
        {
            _result = false;
        }

        return _result;
    }

    ArxmlNode ArxmlReader::GetRootNode(
        std::initializer_list<std::string> children) const
    {
        pugi::xml_node _root{mDocument.root()};

        for (auto &&child : children)
        {
            _root = _root.child(child.c_str());
        }

        ArxmlNode _result(std::move(_root));
        return _result;
    }

    ArxmlNodeRange ArxmlReader::GetNodes(
        std::initializer_list<std::string> children) const
    {
        pugi::xml_node _root{mDocument.root()};

        for (auto &&child : children)
        {
            _root = _root.child(child.c_str());
        }

        auto _range{_root.children()};
        ArxmlNodeRange _result(_range.begin(), _range.end());

        return _result;
    }
}