#include <QTcpServer>
#include <QTcpSocket>
#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <string>
#include <QSqlDatabase>
#include <map>

#include "../Client/itemStats.h"
//Take a look at this file; it includes some tools to
//set up an item based on its data and turn it into a
//byte array. Also turn it into a QJsonObject.
#ifndef __SERVER_H
#define __SERVER_H

struct ownedDB{
    int id;
    QSqlDatabase db;
};

class Server : public QMainWindow
{
	Q_OBJECT
	private:
		QTcpServer* m_server;
		QSet<QTcpSocket*> connected;
		std::string m_board_id;
        QVector<ownedDB> databases;
        int m_port;

        std::map<std::string, std::vector<QJsonObject>*> m_boards;
        std::map<std::string, std::vector<QTcpSocket*>> m_connected_to_board;

	public slots:
		void newConnection();
		void readSocket();
		void disconnect();
	public:
        Server(int port);
		~Server();
		void appendSocket(QTcpSocket* sock);
        void createBoard(QTcpSocket* socket);
		void saveDB(QTcpSocket* socket);
        void deleteDB(QTcpSocket* socket);
        void fullUpdate(QString databaseName, QTcpSocket* socket);
};

#endif
