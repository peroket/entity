
#include "catch/catch.hpp"

#define TAG "[signal]"

#include "signal/Sender.h"
#include "signal/Receiver.h"

enum Signals : size_t {
    BASIC,
    USELESS,
    COPY_REFERENCE
};

DECLARE_SIGNAL(BASIC, int)
DECLARE_SIGNAL(USELESS)
DECLARE_SIGNAL(COPY_REFERENCE, int &, int *)

TEST_CASE("Basic working tests", TAG) {

    Signal::Sender<BASIC> sender;

    SECTION ("receiving") {

        sender.send(0);

        Signal::Receiver<BASIC> receiver;
        Signal::Receiver<USELESS> receiver2;

        // no signal at creation
        CHECK (receiver.hasNewSignal() == false);
        CHECK (receiver2.hasNewSignal() == false);

        sender.send(1);

        // check signal reception
        REQUIRE (receiver.hasNewSignal() == true);
        CHECK (receiver2.hasNewSignal() == false);

        receiver.receive([] (int i) {
            REQUIRE (i == 1);
        });
    }

    SECTION("multiple receivers") {
        Signal::Receiver<BASIC> receiver;
        Signal::Receiver<BASIC> receiver2;

        sender.send(2);

        CHECK (receiver.hasNewSignal() == true);
        CHECK (receiver2.hasNewSignal() == true);

        receiver.receive([] (int i) {
            CHECK (i == 2);
        });

        CHECK (receiver2.hasNewSignal() == true);

        receiver2.receive([] (int i) {
            CHECK (i == 2);
        });
    }

}

TEST_CASE("check copy/reference", TAG) {

    // check copy/reference
    int value1 = 0;
    int value2 = 0;

    Signal::Sender<COPY_REFERENCE> sender;
    Signal::Receiver<COPY_REFERENCE> receiver;

    sender.send(value1, &value2);

    receiver.receive([] (int const &, int * b) {
//        a = 1;
        *b = 2;
    });

    CHECK (value1 == 0);
    CHECK (value2 == 2);

}

TEST_CASE("Test with threads", TAG) {

    SECTION ("sending from thread") {
        Signal::Receiver<BASIC> receiver;

        std::thread thread([] {
            Signal::Sender<BASIC>::send(12);
        });
        thread.join();

        CHECK (receiver.hasNewSignal() == true);

        receiver.receive([] (int i) {
            CHECK (i == 12);
        });
    }

    SECTION ("receiving from thread") {

        bool signalSent = false;

        std::thread thread([&signalSent] {
            Signal::Receiver<BASIC> receiver;

            while(!receiver.hasNewSignal() && !signalSent) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            CHECK (receiver.hasNewSignal() == true);

            receiver.receive([] (int i) {
                CHECK (i == 14);
            });
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));
        Signal::Sender<BASIC>::send(14);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        signalSent = true;

        thread.join();
    }

}














