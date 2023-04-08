#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <mutex>
#include <queue>
#include <atomic>

namespace ara
{
    namespace com
    {
        namespace helper
        {
            /// @brief Thread-safe wrapper around STL queue using locking mechanism
            /// @tparam T Queue element type
            template <typename T>
            class ConcurrentQueue
            {
            private:
                std::mutex mMutex;
                std::queue<T> mQueue;
                std::atomic_size_t mSize;

            public:
                ConcurrentQueue() : mSize{0}
                {
                }

                ~ConcurrentQueue() = default;

                /// @brief Indicate whether the queue is empty or not
                /// @returns True if the queue is empty, otherwise false
                bool Empty() const noexcept
                {
                    return mSize == 0;
                }

                /// @brief Try to insert an element to the queue via moving
                /// @param[in] element Element to be moved into the queue
                /// @return True if the element is moved to the queue successfully, otherwise false
                /// @note The insertion is based on move constructor emplacement rather than pushing.
                bool TryEnqueue(T &&element)
                {
                    std::unique_lock<std::mutex> _lock(mMutex, std::defer_lock);
                    if (_lock.try_lock())
                    {
                        mQueue.emplace(std::move(element));
                        ++mSize;
                        _lock.unlock();
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }

                /// @brief Try to insert an element to the queue via copying
                /// @param[in] element Element to be moved into the queue
                /// @return True if the element is moved to the queue successfully, otherwise false
                /// @note The insertion is based on move constructor emplacement rather than pushing.
                bool TryEnqueue(const T &element)
                {
                    std::unique_lock<std::mutex> _lock(mMutex, std::defer_lock);
                    if (_lock.try_lock())
                    {
                        mQueue.emplace(element);
                        ++mSize;
                        _lock.unlock();
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }

                /// @brief Try to peek an element from the queue by removing it
                /// @param[out] element Element that is moved out from the queue
                /// @returns True if the element is dequeued successfully, otherwise false
                bool TryDequeue(T &element)
                {
                    std::unique_lock<std::mutex> _lock(mMutex, std::defer_lock);
                    if (_lock.try_lock())
                    {
                        element = std::move(mQueue.front());
                        mQueue.pop();
                        --mSize;
                        _lock.unlock();
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            };
        }
    }
}

#endif