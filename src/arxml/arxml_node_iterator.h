#ifndef ARXML_NODE_ITERATOR
#define ARXML_NODE_ITERATOR

#include <utility>
#include "./arxml_node.h"

namespace arxml
{
    /// @brief Read-only access iterator for a range of ARXML nodes
    /// @see ArxmlNode
    class ArxmlNodeIterator
    {
    private:
        pugi::xml_node_iterator mIterator;

    public:
        ArxmlNodeIterator() = delete;

		/// @brief Constructor
		/// @param iterator XML iterator
		explicit ArxmlNodeIterator(pugi::xml_node_iterator iterator) : mIterator{iterator}
        {
        }

		inline bool operator==(const ArxmlNodeIterator& rhs) const
        {
            return mIterator == rhs.mIterator;
        }

		inline bool operator!=(const ArxmlNodeIterator& rhs) const
        {
            return mIterator != rhs.mIterator;
        }

		inline ArxmlNode operator*() const
        {
            pugi::xml_node _xmlNode(*mIterator);
            ArxmlNode _result(std::move(_xmlNode));
            return _result;
        }

		inline ArxmlNodeIterator& operator++()
        {
            ++mIterator;
            return *this;
        }

		inline ArxmlNodeIterator operator++(int)
        {
            ArxmlNodeIterator _retval = *this;
            ++mIterator;
            return _retval;
        }

		inline ArxmlNodeIterator& operator--()
        {
            --mIterator;
            return *this;
        }

		inline ArxmlNodeIterator operator--(int)
        {
            ArxmlNodeIterator _retval = *this;
            --mIterator;
            return _retval;
        }
    };
}

#endif