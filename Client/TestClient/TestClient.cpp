#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
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
    TestClient();
    ~TestClient();

 //   QApplication new_window(argc, argv);
    Window w;

private:

private slots:
    void test_Start();
    void test_textTool();
    void test_lineTool();
    void test_latexTool();
    void test_ellipseTool();
    void test_rectTool();
    void test_arrowTool();
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

void TestClient::test_textTool()
{
    //itemStats(std::string board_id, std::string type, int id, double x, double y, double height, double width, double scenex, double sceney, QColor outline);
    itemStats stat = itemStats("CMSC461","text", 1, 0.0,0.0, 10.1, 10.1, 0.0, 0.0, "fff00");
    ProjectScene* s =  w.get_m_scene();
    ProjectView* v = w.get_m_view();
    int toolNum = 0;

    QCOMPARE(s, s);//s->sendItem(stat);
    toolNum = v->get_m_tool();
    w.get_m_bar()->set_text();
    QVERIFY(toolNum == 0);
}

void TestClient::test_latexTool()
{
    itemStats stat = itemStats("CMSC461","latex", 1, 0.0,0.0, 10.1, 10.1, 0.0, 0.0, "fff00");
    ProjectScene* s =  w.get_m_scene();
    ProjectView* v = w.get_m_view();
    int toolNum = 0;

    QCOMPARE(s, s);//s->sendItem(stat);
    toolNum = v->get_m_tool();
    w.get_m_bar()->set_latex();
    QVERIFY(toolNum > 0);
}

void TestClient::test_lineTool()
{
    itemStats stat = itemStats("CMSC461","line", 1, 0.0,0.0, 10.1, 10.1, 0.0, 0.0, "fff00");
    ProjectScene* s =  w.get_m_scene();
    ProjectView* v = w.get_m_view();
    int toolNum = 0;

   QCOMPARE(s, s);// s->sendItem(stat);
    toolNum = v->get_m_tool();
    w.get_m_bar()->set_line();
    QVERIFY(toolNum > 0);
}

void TestClient::test_ellipseTool()
{
    itemStats stat = itemStats("CMSC461","circle", 1, 0.0,0.0, 10.1, 10.1, 0.0, 0.0, "fff00");
    ProjectScene* s =  w.get_m_scene();
    ProjectView* v = w.get_m_view();
    int toolNum = 0;

    QCOMPARE(s, s);//s->sendItem(stat);
    toolNum = v->get_m_tool();
    w.get_m_bar()->set_circle();
    QVERIFY(toolNum > 0);
}

void TestClient::test_arrowTool()
{
    itemStats stat = itemStats("CMSC461","arrow", 1, 0.0,0.0, 10.1, 10.1, 0.0, 0.0, "fff00");
    ProjectScene* s =  w.get_m_scene();
    ProjectView* v = w.get_m_view();
    int toolNum = 0;

    QCOMPARE(s, s);//s->sendItem(stat);
    toolNum = v->get_m_tool();
    w.get_m_bar()->set_arrow();
    QVERIFY(toolNum > 0);
}

void TestClient::test_rectTool()
{
    itemStats stat = itemStats("CMSC461","rectangle", 1, 0.0,0.0, 10.1, 10.1, 0.0, 0.0, "fff00");
    ProjectScene* s =  w.get_m_scene();
    ProjectView* v = w.get_m_view();
    int toolNum = 0;

    QCOMPARE(s, s);//s->sendItem(stat);
    toolNum = v->get_m_tool();
    w.get_m_bar()->place_rectangle();
    QVERIFY(toolNum > 0);
}

void TestClient::test_Start()
{
    w.setup_window();
    w.get_m_bar()->set_default();
}

QTEST_MAIN(TestClient)

#include "TestClient.moc"
