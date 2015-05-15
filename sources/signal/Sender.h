
#pragma once
#ifndef SIGNAL_EMITTER_H_
#define SIGNAL_EMITTER_H_

#define SIGNAL_THREAD_SUPPORT   // only protection is for Sender::send(), receivers can be used from any threads, but no more than one thread per receiver at once

namespace Signal {

#define DECLARE_SIGNAL(ID, ...) template <typename ChildType> \
                                struct ::Signal::SenderTypes<ID, ChildType> : public internal::SenderBase<ChildType, ## __VA_ARGS__> {};

    template <size_t ID, typename ChildType>
    struct SenderTypes;

    template <size_t ID>
    class Sender : public SenderTypes<ID, Sender<ID> > {

        friend class SenderTypes<ID, Sender>::SenderBase;

    public:
        using SignalPointer         = typename SenderTypes<ID, Sender>::SignalPointer;
        using ConstSignalPointer    = typename SenderTypes<ID, Sender>::ConstSignalPointer;

    private:
        // the must be declared here to have one and only one per signal type

        static SignalPointer s_lastSignal;
#ifdef SIGNAL_THREAD_SUPPORT
        static std::mutex s_mutex;
#endif

    public:
        static ConstSignalPointer getLastSignal();
        static bool hasReceivers();

        using SenderTypes<ID, Sender>::send;

    private:
        static void send(SignalPointer signal);

    };

    // specialize if your type does not support default init (values will not be used)
    template <size_t ID>
    typename Sender<ID>::SignalPointer Sender<ID>::s_lastSignal = std::make_shared<typename SenderTypes<ID, Sender>::SignalValues>();

#ifdef SIGNAL_THREAD_SUPPORT
    template <size_t ID>
    std::mutex Sender<ID>::s_mutex;
#endif

    namespace internal {

        template <typename... Args>
        struct Base {

            struct SignalValues;

            // shared_ptr is capital here, because that way the stored signal values are kept until every receiver received it and is destroyed automatically after that
            using SignalPointer         = std::shared_ptr<SignalValues>;
            using ConstSignalPointer    = std::shared_ptr<SignalValues const>;
            using ValueTypes            = std::tuple<std::decay_t<Args>...>;

            struct SignalValues {
                SignalPointer m_next;
                ValueTypes    m_values;

                SignalValues() = default;
                SignalValues(Args &&... args)
                : m_values(std::forward<Args>(args)...)
                {}
            };

        };

        // specialize SignalValues if there are no parameters, otherwise the default constructor is in conflict
        template <>
        struct Base<>::SignalValues {
            SignalPointer m_next;
            ValueTypes    m_values;

            SignalValues() = default;
        };

        template <typename ChildType, typename... Args>
        class SenderBase : public Base<Args...> {

        protected:
            // if you want references, use std::ref, otherwise same as std thread
            static void send(Args &&... args) {

                // avoid allocation and such if no one listens
                if (ChildType::hasReceivers())
                    ChildType::send(std::make_shared<typename Base<Args...>::SignalValues>(std::forward<Args>(args)...));

            }

        };

    }

    /**************************************************************/
    /**** Inline functions ****************************************/
    /**************************************************************/

    //// Public ////////////////////////////////////////////////////

    template <size_t ID>
    inline auto Sender<ID>::getLastSignal() -> ConstSignalPointer {

        return s_lastSignal;

    }

    template <size_t ID>
    inline bool Sender<ID>::hasReceivers() {

        return !s_lastSignal.unique();

    }

    //// Protected /////////////////////////////////////////////////

    //// Private ///////////////////////////////////////////////////

    template <size_t ID>
    void Sender<ID>::send(SignalPointer signal) {

#ifdef SIGNAL_THREAD_SUPPORT
        std::unique_lock<std::mutex> lock(s_mutex);
#endif
        s_lastSignal->m_next = signal;
        s_lastSignal = std::move(signal);

    }

}

#endif // SIGNAL_EMITTER_H_

