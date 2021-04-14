#include "../../Server.h"
#include <QtTest>
#include <QApplication>
#include <QDataStream>
#include <QSqlQuery>

// add necessary includes here

class ServerShapeTest: public QObject
{
    Q_OBJECT

public:
    ServerShapeTest();
    ~ServerShapeTest();

private slots:
    void testConstructor();
    void testEllipse();
    void testCircle();
    void testLine();
    int main(int argc, char* argv[]);

private:
    Server ser;
    QByteArray buf;
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
};

ServerShapeTest::ServerShapeTest()
{

}

ServerShapeTest::~ServerShapeTest()
{

}

void ServerShapeTest::testConstructor()
{
    QVERIFY(ser.m_board_id == "CMSC461");
}

void ServerShapeTest::testLine()
{
    QVERIFY(ser.m_board_id == "CMSC461");
}

void ServerShapeTest::testCircle()
{
    QVERIFY(ser.m_board_id == "CMSC461");
}

void ServerShapeTest::testEllipse()
{
    QVERIFY(ser.m_board_id == "CMSC461");
}

int ServerShapeTest::main(int argc, char* argv[])
{
    QApplication new_window(argc, argv);
    Server* s = new Server();
    testConstructor();
    testCircle();
    testEllipse();
    testLine();
    return new_window.exec();
}

//QTEST_APPLESS_MAIN(ServerShapeTest)

#include "tst_servershapetest.moc"
