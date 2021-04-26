#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class TestServer : public QObject
{
    Q_OBJECT
public:
    explicit TestServer(QObject *parent = 0);
    void Test();

    void badDataConnect();
    void initTestCase();
    void cleanupTestCase();
    void testConstructor();

    bool testy = true;



signals:

public slots:
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void connected();

    void testGoodEllipse();
    void testGoodLine();
    void testGoodFill();
    void testGoodText();
    void testGoodLatex();
    void testGoodRectangle();
private:
    QTcpSocket *socket;

};

#endif // TESTSERVER_H
