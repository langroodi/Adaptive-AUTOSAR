#include <utility>
#include <sstream>
#include "./arxml_node.h"

namespace arxml
{
    ArxmlNode::ArxmlNode(pugi::xml_node &&node) : mNode{std::move(node)}
    {
    }

    std::string ArxmlNode::GetShortName() const
    {
        const char *cShortNameNode{"SHORT-NAME"};
        std::string _result{mNode.child(cShortNameNode).text().as_string()};
        return _result;
    }

    template <>
    std::string ArxmlNode::GetValue(std::string defaultValue) const
    {
        auto _value{mNode.text().as_string(defaultValue.c_str())};
        std::string _result{_value};

        return _result;
    }

    bool ArxmlNode::TryGetReference(
        const std::string &sourceNode, const std::string &destinationType,
        std::string &referenceNode) const
    {
        const char *cDestinationAttribute{"DEST"};

        bool _result;
        pugi::xml_node _child{mNode.child(sourceNode.c_str())};
        std::string _actualType{
            _child.attribute(cDestinationAttribute).as_string()};

        if (destinationType == _actualType)
        {
            referenceNode = _child.text().as_string();
            _result = true;
        }
        else
        {
            _result = false;
        }

        return _result;
    }

    std::string ArxmlNode::GetContent() const
    {
        std::stringstream _stringStream;
        mNode.print(_stringStream);
        std::string _result{_stringStream.str()};

        return _result;
    }
}