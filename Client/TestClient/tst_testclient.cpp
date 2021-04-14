#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class TestClient : public QObject
{
    Q_OBJECT

public:
    TestClient();
    ~TestClient();

private slots:
    void test_case1();

};

TestClient::TestClient()
{

}

TestClient::~TestClient()
{

}

void TestClient::test_case1()
{

}

QTEST_MAIN(TestClient)

#include "tst_testclient.moc"
