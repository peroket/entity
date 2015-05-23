
#pragma once
#ifndef SIGNAL_MULTIRECEIVER_H_
#define SIGNAL_MULTIRECEIVER_H_

#include "signal/Sender.h"

namespace Signal {

    template <size_t... IDs>
    class MultiReceiver {

    private:
        std::tuple<typename Sender<IDs>::ConstSignalPointer...> m_signals;
//        std::deque<size_t> m_ids; // stores the signal

    public:
        MultiReceiver();
        MultiReceiver(MultiReceiver const &) = default;
        MultiReceiver(MultiReceiver &&) = default;
        ~MultiReceiver() = default;

        bool hasNewSignal() const;

        // call function for all untreated signals (and discards them)
        void receive();
        // call function for most ancient signal only (and discards it)
        void receiveOne();

    private:
        // convert the tuple into function parameter(s) thanks to the second function
//        void apply(typename Sender<ID>::ValueTypes const & values);

//        template <size_t... Ints>
//        void apply(typename Sender<ID>::ValueTypes const & values, std::integer_sequence<size_t, Ints...>);

    };

    /**************************************************************/
    /**** Inline functions ****************************************/
    /**************************************************************/

    //// Public ////////////////////////////////////////////////////

    //// Protected /////////////////////////////////////////////////

    //// Private ///////////////////////////////////////////////////

}

#endif // SIGNAL_MULTIRECEIVER_H_

