#include "Server.h"
#include <iostream>
#include <QDataStream>

Server::Server() : QMainWindow()
{
	m_server = new QTcpServer();
	if(m_server->listen(QHostAddress::LocalHost, 5000)) {
		std::cout << "Server is listening." << std::endl;
	}
	else {
		std::cout << "Failed." << std::endl;
	}
	connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

Server::~Server()
{
}

void Server::newConnection()
{
	while(m_server->hasPendingConnections()) {
		appendSocket(m_server->nextPendingConnection());
	}
}

void Server::appendSocket(QTcpSocket* socket)
{
	connected.insert(socket);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect()));
	std:: cout << "Client with ID " << socket->socketDescriptor() << " connected." << std::endl;
}

void Server::disconnect()
{
	QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
	QSet<QTcpSocket*>::iterator it = connected.find(socket);
	if(it != connected.end()) {
		std::cout << "Client with ID " << socket->socketDescriptor() << " disconnected." << std::endl;
		connected.remove(*it);
	}
	socket->deleteLater();
}

void Server::readSocket()
{
	QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
	QByteArray buf;
	QDataStream sockstream(socket);
	sockstream.startTransaction();
	sockstream >> buf;
	if(!sockstream.commitTransaction()) {
		std::cout << "Awaiting additional data." << std::endl;
		return;
	}
	std::cout << "Received " << buf.toStdString() << std::endl;
}

