#include <QCoreApplication>
#include "TestServer.h"
#include "../Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

  //  int port = 5000;
   // Server m = Server(port);

    TestServer cTest;
    cTest.badDataConnect();
    cTest.Test();

    return a.exec();
}
