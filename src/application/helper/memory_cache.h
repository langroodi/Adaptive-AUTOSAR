#ifndef MEMORY_CACHE_H
#define MEMORY_CACHE_H

#include <chrono>
#include <map>

namespace application
{
    namespace helper
    {
        /// @brief Memory cache helper class with expiration time
        /// @tparam K Cache key type
        /// @tparam T Cached value type
        /// @note The class is not thread-safe.
        template <typename K, typename T>
        class MemoryCache
        {
        private:
            const std::chrono::seconds cLifetime;
            std::map<K, std::pair<const std::chrono::system_clock::time_point, const T>> mCachedItems;

        public:
            /// @brief Constructor
            /// @param lifetime Cached item lifetime in seconds
            explicit MemoryCache(std::chrono::seconds lifetime) : cLifetime{lifetime}
            {
            }

            /// @brief Add an item to the cache
            /// @param key Item cache key
            /// @param item Item to be cached
            void Add(K key, const T &item)
            {
                mCachedItems.emplace(
                    key, std::make_pair(std::chrono::system_clock::now(), item));
            }

            /// @brief Try to get an item from the cache
            /// @param[in] key Item cache key
            /// @param[out] item Cached item
            /// @return True if the item exists within the cache by the given key; otherwise false
            /// @note The 'item' argument will be untouched while returning 'false'.
            bool TryGet(K key, T &item)
            {
                auto _cachedItemItr{mCachedItems.find(key)};

                if (_cachedItemItr != mCachedItems.end())
                {
                    const auto cNow{std::chrono::system_clock::now()};
                    if (cNow - _cachedItemItr->second.first < cLifetime)
                    {
                        item = _cachedItemItr->second.second;
                        return true;
                    }
                    else
                    {
                        // Remove the item if it is already expired
                        mCachedItems.erase(_cachedItemItr);
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        };
    }
}

#endif