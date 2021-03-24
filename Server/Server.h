#include <QTcpServer>
#include <QTcpSocket>
#include <QMainWindow>

#ifndef __SERVER_H
#define __SERVER_H
class Server : public QMainWindow
{
	Q_OBJECT
	private:
		QTcpServer* m_server;
		QSet<QTcpSocket*> connected;
	public slots:
		void newConnection();
		void readSocket();
		void disconnect();
	public:
		Server();
		~Server();
		void appendSocket(QTcpSocket* sock);
};
#endif
