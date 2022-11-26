#include <utility>
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
}