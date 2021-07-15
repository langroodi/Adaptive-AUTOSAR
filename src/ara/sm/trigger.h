#ifndef TRIGGER_H
#define TIGGER_H

#include <stdint.h>
#include <functional>

namespace ara
{
    namespace sm
    {
        enum class FunctionGroupStates : uint8_t
        {
            kOff = 0,
            kRunning = 1,
            kUpdate = 2,
            kVerify = 3
        };

        template <typename T>
        class Tigger
        {
        public:
            Tigger();
            ~Tigger();
            Tigger(const Tigger &) = delete;
            Tigger(Tigger &&) = delete;
            Tigger &operator=(const Tigger &) = delete;
            Tigger &operator=(Tigger &&) = delete;
            void write(T state);
        };

        template <typename T>
        class Notifier
        {
        public:
            Notifier();
            ~Notifier();
            Notifier(const Notifier &) = delete;
            Notifier(Notifier &&) = delete;
            Notifier &operator=(const Notifier &) = delete;
            Notifier &operator=(Notifier &&) = delete;
            T read() const noexcept;
            void subscribe(std::function < void<T> handler);
        };

        template <typename T>
        class TiggerIn
        {
        public:
            TiggerIn() noexcept;
            ~TiggerIn();
            TiggerIn(const TiggerIn &) = delete;
            TiggerIn(TiggerIn &&) = delete;
            TiggerIn &operator=(const TiggerIn &) = delete;
            TiggerIn &operator=(TiggerIn &&) = delete;
            Tigger Tigger() const noexcept;
        };

        template <typename T>
        class TiggerOut
        {
        public:
            TiggerOut();
            ~TiggerOut();
            TiggerOut(const TiggerOut &) = delete;
            TiggerOut(TiggerOut &&) = delete;
            TiggerOut &operator=(const TiggerOut &) = delete;
            TiggerOut &operator=(TiggerOut &&) = delete;
            Notifier Notifier() const noexcept;
        };

        template <typename T>
        class TiggerInOut
        {
        public:
            TiggerInOut();
            ~TiggerInOut();
            TiggerOut(const TiggerInOut &) = delete;
            TiggerInOut(TiggerOut &&) = delete;
            TiggerInOut &operator=(const TiggerInOut &) = delete;
            TiggerInOut &operator=(TiggerInOut &&) = delete;
            Tigger Tigger() const noexcept;
            Notifier Notifier() const noexcept;
        };
    }
}

#endif