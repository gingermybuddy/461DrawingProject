#include "../Server.h"
#include <QtTest>
#include <QCoreApplication>
#include "TestServer.h"


// add necessary includes here
/*
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
*/
/*
TestServer::TestServer()
{

}

TestServer::~TestServer()
{

}
*/

TestServer::TestServer(QObject *parent) :
    QObject(parent)
{
}

void TestServer::initTestCase()
{

}

void TestServer::cleanupTestCase()
{

}
/*
void TestServer::testConstructor()
{
    int argc=1;
    char* argv[1];
    QApplication new_window(argc, argv);
    Server* ser = new Server();

    QVERIFY(ser != NULL);

    delete ser;
}
*/
void TestServer::badDataConnect()
{
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 5000);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";

        // send
        socket->write("hello server\r\n\r\n\r\n\r\n");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << "Reading: " << socket->bytesAvailable();

        qDebug() << socket->readAll();

        socket->close();
    }
    else
    {
        qDebug() << "Not connected!";
    }

    // sent

    // got

    // closed
}

//QTEST_MAIN(TestServer)

//#include "tst_testserver.moc"

void TestServer::Test()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(testGoodEllipse()), this, SLOT(testGoodEllipse()));
    connect(socket, SIGNAL(testGoodRectangle()), this, SLOT(testGoodRectangle()));
    connect(socket, SIGNAL(testGoodFill()), this, SLOT(testGoodFill()));
    connect(socket, SIGNAL(testGoodLine()), this, SLOT(testGoodLine()));
    connect(socket, SIGNAL(testGoodLatex()), this, SLOT(testGoodLatex()));
    connect(socket, SIGNAL(testGoodText()), this, SLOT(testGoodText()));

    qDebug() << "Connecting,..";

    socket->connectToHost(QHostAddress::LocalHost, 5000);

    if(!socket->waitForDisconnected(1000))
    {
        qDebug() << "Error: " << socket->errorString();
    }

}

void TestServer::connected()
{
    qDebug() << "Connected!";

    //socket->write();
}

void TestServer::disconnected()
{
    qDebug() << "Disconnected!";
}

void TestServer::bytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes;
}

void TestServer::readyRead()
{
    qDebug() << "Reading...";
    qDebug() << socket->readAll();
}

void TestServer::testGoodEllipse()
{
    //std::string answer;
    qDebug() << "Testing Ellispe";
    if(false){
        //answer = "";
        qDebug() << "";

    }else{
        qDebug() << "Failed ellipse data ";
    }
}

void TestServer::testGoodFill()
{
    //std::string answer;
    qDebug() << "Testing Circle";
    if(false){
        //answer = "";
        qDebug() << "";

    }else{
        qDebug() << "Failed circle data ";
    }
}

void TestServer::testGoodRectangle()
{
    //std::string answer;
    qDebug() << "Testing Rectangle...";
    if(false){
        //answer = "";
        qDebug() << "";

    }else{
        qDebug() << "Failed rectangle data ";
    }
}

void TestServer::testGoodLine()
{
    //std::string answer;
    qDebug() << "Testing Line ...";
    if(false){
        //answer = "";
        qDebug() << "";

    }else{
        qDebug() << "Failed line data ";
    }
}

void TestServer::testGoodText()
{
    qDebug() << "Testing Text ...";
    if(false){
        //answer = "";
        qDebug() << "";

    }else{
        qDebug() << "Failed text data ";
    }
}

void TestServer::testGoodLatex()
{
    qDebug() << "Testing LaTeX ...";
    if(false){
        //answer = "";
        qDebug() << "";

    }else{
        qDebug() << "Failed LaTeX data ";
    }
}
