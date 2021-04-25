#include <QCoreApplication>
#include "TestServer.h"
#include "../Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    Server m;

    TestServer cTest;
    cTest.Connect();
    cTest.Test();

    return a.exec();
}
