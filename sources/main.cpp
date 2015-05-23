#include "MainWindow.h"
#include <QApplication>

#include "Log.h"

#include "signal/Sender.h"
#include "signal/Receiver.h"
#include "signal/MultiReceiver.h"


DECLARE_SIGNAL(0, int)

int main(int argc, char **argv) {

    Signal::Sender<0> send0;
    Signal::Receiver<0> rec0;

    send0.send(1);

    rec0.receive([] (int a) {
        Log::out("receive 0 : {}", a);
    });

    return 0;
}

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
//}
