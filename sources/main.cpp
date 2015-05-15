#include "MainWindow.h"
#include <QApplication>

#include "Log.h"

#include "signal/Sender.h"
#include "signal/Receiver.h"
#include "signal/MultiReceiver.h"

struct Test {
    Test() = default;
    Test(int) {}
};

DECLARE_SIGNAL(0, int, Test)
DECLARE_SIGNAL(1, int *)
DECLARE_SIGNAL(5, char const *)

DECLARE_SIGNAL(4)

int main(int argc, char **argv) {

    Signal::Sender<0> send0;
    Signal::Sender<0> send0_2;
    Signal::Sender<1> send1;
    Signal::Sender<5> send5;

    Signal::Sender<4> send4;

    auto rec0 = Signal::make_receiver<0>([] (int a, Test const & b) {
        Log::out("receive 0 : {} {}", a, a);
    });
    auto rec1 = Signal::make_receiver<1>([] (int * a) {
        Log::out("receive 1 : {}", *a);
        *a = 14;
    });
    auto rec5 = Signal::make_receiver<5>([] (char const * a) {
        Log::out("receive 5 : {}", a);
    });

    send0.send(1, 2.f);
    send0_2.send(2, 5.f);
    rec0.receive();

    auto rec0_2 = Signal::make_receiver<0>([] (int a, Test const & b) {
        Log::out("receive_2 0 : {} {}", a, a);
    });
    Signal::Sender<0>::send(12, 4);
    send0.send(1, 3.f);
    send0_2.send(2, 6.f);
    rec0.receive();
    rec0_2.receive();

    auto rec4 = Signal::make_receiver<4>([] {
        Log::out("receive 4");
    });
    send4.send();
    rec4.receive();

    int a = 13;
    send1.send(&a);
    rec1.receive();
    Log::out("modified 1 : {}", a);

    send5.send("haaaaaaa !!");
    rec5.receive();

    return 0;
}

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
//}
