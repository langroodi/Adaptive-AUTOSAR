#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <functional>
#include <vector>

namespace ara
{
    namespace sm
    {
        template <typename T>
        class Notifier
        {
        private:
            using NotificationHandler = std::function<void(T)>;
            T mState;
            std::vector<NotificationHandler> mSubsribers;
            
        public:
            Notifier();
            ~Notifier();
            Notifier(const Notifier &) = delete;
            Notifier(Notifier &&) = delete;
            Notifier &operator=(const Notifier &) = delete;
            Notifier &operator=(Notifier &&) = delete;
            T Read() const noexcept;
            void Subscribe(NotificationHandler handler);
        };
    }
}
#endif