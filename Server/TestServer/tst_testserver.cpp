#include "../Server.h"
#include <QtTest>
#include <QCoreApplication>


// add necessary includes here

class TestServer : public QObject
{
    Q_OBJECT

public:
//    TestServer();
//    ~TestServer();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testConstructor();

};
/*
TestServer::TestServer()
{

}

TestServer::~TestServer()
{

}
*/
void TestServer::initTestCase()
{

}

void TestServer::cleanupTestCase()
{

}

void TestServer::testConstructor()
{
    int argc=1;
    char* argv[1];
    QApplication new_window(argc, argv);
    Server* ser = new Server();

    QVERIFY(ser != NULL);

    delete ser;
}

QTEST_MAIN(TestServer)

#include "tst_testserver.moc"
