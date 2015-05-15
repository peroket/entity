
#pragma once
#ifndef SIGNAL_MULTIRECEIVER_H_
#define SIGNAL_MULTIRECEIVER_H_

#include "signal/Sender.h"

namespace Signal {

    template <template <size_t, typename> class... Receiver>
    class MultiReceiver {

    private:
//        typename Sender<ID>::ConstSignalPointer m_lastSignalTreated;
//        Function                                m_function;

    public:
//        MultiReceiver(Function f);
        MultiReceiver() = default;
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

