#include "../Server.h"
#include <QtTest>
#include <QCoreApplication>
#include "TestServer.h"
#include <QJsonObject>
#include <QDataStream>




TestServer::TestServer(QObject *parent) :
    QObject(parent)
{
}

void TestServer::initTestCase()
{

}

void TestServer::cleanupTestCase()
{
    delete socket;
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
        QDataStream sockstream(socket);
        sockstream.startTransaction();
        sockstream.commitTransaction();
        qDebug() << "Bad Connection Passed";


    }
    else
    {
        qDebug() << "Server Down :(";
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
/*    connect(socket, SIGNAL(namespace::TestServer::testGoodEllipse()), this, SLOT(testGoodEllipse()));
    connect(socket, SIGNAL(testGoodRectangle()), this, SLOT(testGoodRectangle()));
    connect(socket, SIGNAL(testGoodFill()), this, SLOT(testGoodFill()));
    connect(socket, SIGNAL(testGoodLine()), this, SLOT(testGoodLine()));
    connect(socket, SIGNAL(testGoodLatex()), this, SLOT(testGoodLatex()));
    connect(socket, SIGNAL(testGoodText()), this, SLOT(testGoodText()));
*/
    qDebug() << "Connecting,..";

    socket->connectToHost(QHostAddress::LocalHost, 5000);

    if(!socket->waitForDisconnected(1000))
    {
        qDebug() << "Error: " << socket->errorString();
    }

    //qDebug() << "Bad Connection Passed";
}

void TestServer::connected()
{
    //QJsonDocument* m_starting_file;

    qDebug() << "Connected!";
    QJsonObject boardcon;
    boardcon.insert("board_connection", QJsonValue(QString::fromStdString("CMSC461")));
/*
    if(m_starting_file != nullptr) {
        QJsonDocument loader = *m_starting_file;
        boardcon.insert("load_file", QJsonValue(loader.object()));
    }
*/
    QByteArray boardreq = QJsonDocument(boardcon).toJson();
    QDataStream sockstream(socket);

    sockstream.startTransaction();
    sockstream << boardreq;
    sockstream.commitTransaction();
    testGoodEllipse();
    testGoodFill();
    testGoodLatex();
    testGoodLine();
    testGoodRectangle();
    testGoodText();

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
    QByteArray buf;

    //Data stream to get it from the socket.
    QDataStream sockstream(socket);
    sockstream.startTransaction();
    sockstream >> buf;
    sockstream.commitTransaction();

    //std::string answer;
    qDebug() << "Testing Ellispe";
    if(testy){
        //answer = "";
        qDebug() << "";

    }else{
        qDebug() << "Failed ellipse data ";
    }
}

void TestServer::testGoodFill()
{
    QByteArray buf;

    //Data stream to get it from the socket.
    QDataStream sockstream(socket);
    sockstream.startTransaction();
    sockstream >> buf;
    sockstream.commitTransaction();

    //std::string answer;
    qDebug() << "Testing Circle";
    if(testy){
        //answer = "";
        qDebug() << "Passed Circle";

    }else{
        qDebug() << "Failed circle data ";
    }
}

void TestServer::testGoodRectangle()
{
    QByteArray buf;

    //Data stream to get it from the socket.
    QDataStream sockstream(socket);
    sockstream.startTransaction();
    sockstream >> buf;
    sockstream.commitTransaction();

    //std::string answer;
    qDebug() << "Testing Rectangle...";
    if(testy){
        //answer = "";
        qDebug() << "Passed Rectangle";

    }else{
        qDebug() << "Failed rectangle data ";
    }
}

void TestServer::testGoodLine()
{
    QByteArray buf;

    //Data stream to get it from the socket.
    QDataStream sockstream(socket);
    sockstream.startTransaction();
    sockstream >> buf;
    sockstream.commitTransaction();

    //std::string answer;
    qDebug() << "Testing Line ...";
    if(testy){
        //answer = "Passed Line";
        qDebug() << "";

    }else{
        qDebug() << "Failed line data ";
    }
}

void TestServer::testGoodText()
{
/*
    QJsonObject d;
    QString text = d.value("text").toString();
    QGraphicsTextItem* t = addText(text);
    QJsonObject start = d.value("start").toObject();
    t->setPos(start.value("x").toDouble(), start.value("y").toDouble());
    t->setDefaultTextColor(QColor(d.value("color").toString()));
    t->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    t->setCursor(Qt::PointingHandCursor);
    t->setData(0, d.value("sid").toInt());
    t->setData(1, "text");
    QJsonObject pos = d.value("scenepos").toObject();
*/

    QByteArray buf;

    //Data stream to get it from the socket.
    QDataStream sockstream(socket);
    sockstream.startTransaction();
    sockstream >> buf;


    qDebug() << "Testing Text ...";
    if(testy){
        //answer = "";
        qDebug() << "Passed Text";

    }else{
        qDebug() << "Failed text data ";
    }
}

void TestServer::testGoodLatex()
{
    QByteArray buf;

    //Data stream to get it from the socket.
    QDataStream sockstream(socket);
    sockstream.startTransaction();
    sockstream >> buf;

    qDebug() << "Testing LaTeX ...";
    if(testy){
        //answer = "";
        qDebug() << "Passed LaTeX";

    }else{
        qDebug() << "Failed LaTeX data ";
    }
}
