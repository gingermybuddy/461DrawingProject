#include <QtTest>
#include <QCoreApplication>
#include <QApplication>
#include "../projectScene.h"
#include "../toolBar.h"
#include "../projectView.h"
#include "../window.h"
#include "../itemStats.h"

// add necessary includes here

class TestClient : public QObject
{
    Q_OBJECT

public:
    TestClient(int argc, char* argv);
    ~TestClient();

 //   QApplication new_window(argc, argv);
 //   Window w;

private slots:
    void test_case1();
//    int argc = 1;
//    char* argv;
};

TestClient::TestClient(int argc, char* argv)
{
    QApplication new_window(argc, argv);
    Window w;
}

TestClient::~TestClient()
{

}

void TestClient::test_case1()
{

}

QTEST_MAIN(TestClient)

#include "tst_testclient.moc"
