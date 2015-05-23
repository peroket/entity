
#include "catch.hpp"

#define TAG "[signal]"

#include "signal/Sender.h"
#include "signal/Receiver.h"

DECLARE_SIGNAL(0u, int)
DECLARE_SIGNAL(1u)

TEST_CASE("Basic working tests", TAG) {

    Signal::Sender<0u> sender;

    sender.send(0);

    Signal::Receiver<0u> receiver;
    Signal::Receiver<1u> receiver2;

    // no signal at creation
    REQUIRE (receiver.hasNewSignal() == false);
    REQUIRE (receiver2.hasNewSignal() == false);

    sender.send(1);

    // check signal reception
    REQUIRE (receiver.hasNewSignal() == true);
    REQUIRE (receiver2.hasNewSignal() == false);

    receiver.receive([] (int i) {
        REQUIRE (i == 1);
    });

    // check multiple receivers
    Signal::Receiver<0u> receiver3;

    sender.send(2);

    REQUIRE (receiver.hasNewSignal() == true);
    REQUIRE (receiver3.hasNewSignal() == true);

    receiver.receive([] (int i) {
        REQUIRE (i == 2);
    });

    REQUIRE (receiver3.hasNewSignal() == true);

    receiver3.receive([] (int i) {
        REQUIRE (i == 2);
    });

}

TEST_CASE("Test with threads", TAG) {

    Signal::Sender<0u> sender;
    // TODO

}














