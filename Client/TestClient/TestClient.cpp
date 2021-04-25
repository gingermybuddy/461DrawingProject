#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QtTest>
/*
#include <QCoreApplication>
#include <QApplication>
#include "../projectScene.h"
#include "../toolBar.h"
#include "../projectView.h"
#include "../window.h"
#include "../itemStats.h"
*/

// add necessary includes here

class TestClient : public QObject
{
    Q_OBJECT

public:
    TestClient();
    ~TestClient();

 //   QApplication new_window(argc, argv);
 //   Window w;

private slots:
    void test_case1();
//    int argc = 1;
//    char* argv;
};

TestClient::TestClient()
{
   // QApplication new_window(argc, argv);
   // Window w;
}

TestClient::~TestClient()
{

}

void TestClient::test_case1()
{

}

QTEST_MAIN(TestClient)

#include "TestClient.moc"
