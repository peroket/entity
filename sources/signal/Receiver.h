
#pragma once
#ifndef SIGNAL_RECEIVER_H_
#define SIGNAL_RECEIVER_H_

#include "signal/Sender.h"

namespace Signal {

    template <size_t ID>
    class Receiver {

    private:
        typename Sender<ID>::ConstSignalPointer m_lastSignalTreated;

    public:
        Receiver();
        Receiver(Receiver const &) = default;
        Receiver(Receiver &&) = default;
        ~Receiver() = default;

        bool hasNewSignal() const;

        // call function for all untreated signals (and discards them)
        template <typename Function>
        void receive(Function f);
        // call function for most ancient signal only (and discards it)
        template <typename Function>
        void receiveOne(Function f);

    private:
        // convert the tuple into function parameter(s) thanks to the second function
        template <typename Function, size_t... Ints>
        void apply(Function f, typename Sender<ID>::ValueTypes const & values, std::integer_sequence<size_t, Ints...>);

    };

    /**************************************************************/
    /**** Inline functions ****************************************/
    /**************************************************************/

    template <size_t ID>
    Receiver<ID>::Receiver()
    : m_lastSignalTreated(Sender<ID>::getLastSignal())
    {}

    //// Public ////////////////////////////////////////////////////

    template <size_t ID>
    inline bool Receiver<ID>::hasNewSignal() const {

        return static_cast<bool>(m_lastSignalTreated->m_next);

    }

    template <size_t ID>
    template <typename Function>
    void Receiver<ID>::receive(Function f) {

        while (m_lastSignalTreated->m_next) {
            m_lastSignalTreated = m_lastSignalTreated->m_next;
            apply(f, m_lastSignalTreated->m_values, std::make_index_sequence<std::tuple_size<typename Sender<ID>::ValueTypes>::value>());
        }

    }

    template <size_t ID>
    template <typename Function>
    void Receiver<ID>::receiveOne(Function f) {

        if (m_lastSignalTreated->m_next) {
            m_lastSignalTreated = m_lastSignalTreated->m_next;
            apply(f, m_lastSignalTreated->m_values, std::make_index_sequence<std::tuple_size<typename Sender<ID>::ValueTypes>::value>());
        }

    }

    //// Protected /////////////////////////////////////////////////

    template <size_t ID>
    template <typename Function, size_t... Ints>
    inline void Receiver<ID>::apply(Function f, typename Sender<ID>::ValueTypes const & values, std::integer_sequence<size_t, Ints...>) {

        f(std::get<Ints>(values)...);

    }

    //// Private ///////////////////////////////////////////////////

}

#endif // SIGNAL_RECEIVER_H_

