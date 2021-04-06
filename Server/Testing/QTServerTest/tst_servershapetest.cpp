#include <QtTest>

// add necessary includes here

class ServerShapeTest : public QObject
{
    Q_OBJECT

public:
    ServerShapeTest();
    ~ServerShapeTest();

private slots:
    void test_case1();

};

ServerShapeTest::ServerShapeTest()
{

}

ServerShapeTest::~ServerShapeTest()
{

}

void ServerShapeTest::test_case1()
{

}

QTEST_APPLESS_MAIN(ServerShapeTest)

#include "tst_servershapetest.moc"
