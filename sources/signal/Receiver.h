
#pragma once
#ifndef SIGNAL_RECEIVER_H_
#define SIGNAL_RECEIVER_H_

#include "signal/Sender.h"

namespace Signal {

    template <size_t ID, typename Function>
    class Receiver {

    private:
        typename Sender<ID>::ConstSignalPointer m_lastSignalTreated;
        Function                                m_function;

    public:
        Receiver(Function f);
        Receiver(Receiver const &) = default;
        Receiver(Receiver &&) = default;
        ~Receiver() = default;

        bool hasNewSignal() const;

        // call function for all untreated signals (and discards them)
        void receive();
        // call function for most ancient signal only (and discards it)
        void receiveOne();

    private:
        // convert the tuple into function parameter(s) thanks to the second function
        void apply(typename Sender<ID>::ValueTypes const & values);

        template <size_t... Ints>
        void apply(typename Sender<ID>::ValueTypes const & values, std::integer_sequence<size_t, Ints...>);

    };

    // convenience function to create a receiver without knowing the function type
    template <size_t ID, typename Function>
    auto make_receiver(Function f) {

        return Receiver<ID, Function>(f);

    }

    /**************************************************************/
    /**** Inline functions ****************************************/
    /**************************************************************/

    template <size_t ID, typename Function>
    Receiver<ID, Function>::Receiver(Function f)
    : m_lastSignalTreated(Sender<ID>::getLastSignal())
    , m_function(f)
    {}

    //// Public ////////////////////////////////////////////////////

    template <size_t ID, typename Function>
    inline bool Receiver<ID, Function>::hasNewSignal() const {

        return m_lastSignalTreated->m_next;

    }

    template <size_t ID, typename Function>
    void Receiver<ID, Function>::receive() {

        while (m_lastSignalTreated->m_next) {
            m_lastSignalTreated = m_lastSignalTreated->m_next;
            apply(m_lastSignalTreated->m_values);
        }

    }

    template <size_t ID, typename Function>
    void Receiver<ID, Function>::receiveOne() {

        if (m_lastSignalTreated->m_next) {
            m_lastSignalTreated = m_lastSignalTreated->m_next;
            apply(m_lastSignalTreated->m_values);
        }

    }

    //// Protected /////////////////////////////////////////////////

    template <size_t ID, typename Function>
    inline void Receiver<ID, Function>::apply(typename Sender<ID>::ValueTypes const & values) {

        apply(values, std::make_index_sequence<std::tuple_size<typename Sender<ID>::ValueTypes>::value>());

    }

    template <size_t ID, typename Function>
    template <size_t... Ints>
    inline void Receiver<ID, Function>::apply(typename Sender<ID>::ValueTypes const & values, std::integer_sequence<size_t, Ints...>) {

        m_function(std::get<Ints>(values)...);

    }

    //// Private ///////////////////////////////////////////////////

}

#endif // SIGNAL_RECEIVER_H_

