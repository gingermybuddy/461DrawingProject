#include <QtTest/QtTest>
#include "../../Server.h"

// add necessary includes here

class ServerShapeTest: public QObject
{
    Q_OBJECT

public:
    ServerShapeTest();
    ~ServerShapeTest();

private slots:
    void testConstructor();
    void init();

private:
    Server ser;
};

ServerShapeTest::ServerShapeTest()
{

}

ServerShapeTest::~ServerShapeTest()
{

}

void ServerShapeTest::testConstructor()
{
    QVERIFY(true);
}

void ServerShapeTest::init()
{
    ser = Server();
}

QTEST_APPLESS_MAIN(ServerShapeTest)

#include "tst_servershapetest.moc"
