#ifndef TRIGGER_H
#define TRIGGER_H

namespace ara
{
    namespace sm
    {
        template <typename T>
        class Trigger
        {
        private:
            T mState;
            
        public:
            Trigger();
            ~Trigger();
            Trigger(const Trigger &) = delete;
            Trigger(Trigger &&) = delete;
            Trigger &operator=(const Trigger &) = delete;
            Trigger &operator=(Trigger &&) = delete;
            void Write(T state);
        };
    }
}

#endif