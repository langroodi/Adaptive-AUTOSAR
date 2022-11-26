#ifndef ARXML_NODE_RANGE_H
#define ARXML_NODE_RANGE_H

#include "./arxml_node_iterator.h"

#include <vector>

namespace arxml
{
    /// @brief Read-only range of ARXML nodes
    /// @see ArxmlNode
    class ArxmlNodeRange
    {
    private:
        const ArxmlNodeIterator mBegin;
        const ArxmlNodeIterator mEnd;

    public:
        /// @brief Constructor
        /// @param begin Beginning XML iterator
        /// @param end Ending XML interator
        ArxmlNodeRange(
            pugi::xml_node_iterator begin, pugi::xml_node_iterator end) : mBegin{begin},
                                                                          mEnd{end}
        {
        }

        /// @brief Constant iterator to the beginning of the range
        /// @return ARXML nodes range beginning iterator
        inline ArxmlNodeIterator begin() const
        {
            return mBegin;
        }

        /// @brief Constant iterator to the end of the range
        /// @return ARXML nodes range ending iterator
        inline ArxmlNodeIterator end() const
        {
            return mEnd;
        }
    };
}

#endif