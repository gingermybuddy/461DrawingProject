#include "Server.h"
#include "../Client/itemStats.h"
#include <iostream>
#include <QDataStream>
#include <QSqlQuery>

Server::Server() : QMainWindow()
{
	//Sets up the server itself.
	m_server = new QTcpServer();

	//Sets up the address and port number to listen on.


	//Both numbers are currently fixed, but we can make it
	//an input later.
	if(m_server->listen(QHostAddress::LocalHost, 5000)) {
		std::cout << "Server is listening." << std::endl;
	}
	else {
		std::cout << "Failed." << std::endl;
	}
	//The board ID right now is also a constant.
	//This is also set on the client.
	m_board_id = "CMSC461";

	//The server will give out a 'signal' when it receives
	//a new connection. This connects that signal to the
	//function 'newConnection()'.
	connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

Server::~Server()
{
	delete m_server;
}

void Server::newConnection()
{
	//Checks for pending connections, then calls
	//the other function that actually adds the socket
	//to the list of sockets we have.
	while(m_server->hasPendingConnections()) {
		appendSocket(m_server->nextPendingConnection());
	}
}


void Server::appendSocket(QTcpSocket* socket)
{
	//'connected' is the vector of sockets. All current clients
	//are stored in that vector.
	connected.insert(socket);

	//Same deal as before. This connects the signals readyRead()
	//and disconnected() to the functions we have below.
	//readyRead signifies that it's got the data and it's, well,
	//ready to read it. Disconnected is the same thing; just
	//tells if the client has been disconnected.
	connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect()));
	std:: cout << "Client with ID " << socket->socketDescriptor() << " connected." << std::endl;
}

void Server::disconnect()
{
	//Black voodoo magic with a cast to determine which socket
	//it was.
	QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

	saveDB(socket);
    deleteDB(socket);

	//Finds the socket.
	QSet<QTcpSocket*>::iterator it = connected.find(socket);
	if(it != connected.end()) {
		std::cout << "Client with ID " << socket->socketDescriptor() << " disconnected." << std::endl;
		connected.remove(*it);
	}
	//Removing a socket means you have to call THIS command
	//instead of just a delete.
	socket->deleteLater();
}

void Server::readSocket()
{
	//More black magic.
	QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

	//The QByteArray here is the array of bytes that the socket
	//has just sent to the server. The array is formatted as a JSON
	//object. The client sends out this JSON object in the exact
	//format that is specified in the jsonExamples folder.
	QByteArray buf;

	//Data stream to get it from the socket.
	QDataStream sockstream(socket);
	sockstream.startTransaction();
	sockstream >> buf;

	//If it's not done sending data yet, nothing happens.
	if(!sockstream.commitTransaction()) {
		std::cout << "Awaiting additional data." << std::endl;
		return;
	}
    if(buf.toStdString() == "createBoard"){
        createBoard(socket);
    }
	//If it gets to here, the data has been received and is
	//ready to be parsed. Right now, all it does is turn it into
	//a string and outputs it to console.
	std::cout << "Received " << buf.toStdString() << std::endl;
	
	//This casts the byte array into a Qt style JSON object.
	//You can get keys and values out of this the same way
	//you would anything else. Qt docs will have more specific info.

	QJsonDocument doc = QJsonDocument::fromJson(buf);
	QJsonObject obj = doc.object();

}

void Server::createBoard(QTcpSocket* socket)
{
    //Create a QSqlDatabsae named db
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //either connect to the database named CMSC461.db or create it if it doesn't exist
    db.setDatabaseName("CMSC461.db");
    db.open();
    //Create and QSqlQuery pointer to be used to execute commands
    QSqlQuery* dbQuery = new QSqlQuery;



    //bid = Board ID
    //sid = Shape ID
    //cid = Client/User ID
    dbQuery->exec("CREATE TABLE Ellipse (bid int, sid int, x1 int, x2 int, y1 int, y2 int, color string, cid int);");
    dbQuery->exec("CREATE TABLE Line (bid int, sid int, x1 int, x2 int, y1 int, y2 int, color string, cid int);");
    dbQuery->exec("CREATE TABLE Rect (bid int, sid int, x int, y int, width int, height int, color string, cid int);");

    ownedDB newDB;
    newDB.id = socket->socketDescriptor();
    newDB.db = db;

    databases.push_back(newDB);
}

void Server::saveDB(QTcpSocket* socket)
{
	db.setDatabaseName("CMSC461.db");
	db.open();
	QByteArray everything = itemStats();

	QSqlQuery elQuery("SELECT * FROM Ellipse");
	QByteArray ellipses = itemStats();
	while(elQuery.next()){
		QByteArray data = itemStats("CMSC461","ellipse",elQuery.value(1),elQuery.value(2),elQuery.value(3),elQuery.value(4),elQuery.value(5),elQuery.value(6).toString()).byteData();
		QJsonDocument doc = QJsonDocument::fromJson(data);
		QJsonObject obj = doc.object();
		data = QJsonDocument::toJson(obj);
		ellipses.append(data);
	}
	everything += "{\"Ellipse\":{"
	everything.append(ellipses);

	QSqlQuery liQuery("SELECT * FROM Line");
	QByteArray lines = itemStats();
	while(liQuery.next()){
		QByteArray data = itemStats("CMSC461","line",liQuery.value(1),liQuery.value(2),liQuery.value(3),liQuery.value(4),liQuery.value(5),liQuery.value(6).toString()).byteData();
		QJsonDocument doc = QJsonDocument::fromJson(data);
		QJsonObject obj = doc.object();
		data = QJsonDocument::toJson(obj);
		lines.append(data);
	}
	everything += "}\"Line\":{"
	everything.append(lines);

	QSqlQuery reQuery("SELECT * FROM Rect");
	QByteArray rectangles = itemStats();
	while(liQuery.next()){
		QByteArray data = itemStats("CMSC461","rect",reQuery.value(1),reQuery.value(2),reQuery.value(3),reQuery.value(4),reQuery.value(5),reQuery.value(6).toString()).byteData();
		QJsonDocument doc = QJsonDocument::fromJson(data);
		QJsonObject obj = doc.object();
		data = QJsonDocument::toJson(obj);
		rectangles.append(data);
	}
	everything += "}\"Rect\":{"
	everything.append(rectangles);

	//cout << "{Ellipse:{" << elQuery.result() << "}Line:{" << liQuery.result() << "}Rect:{" << reQuery.result() << "}";
}

void Server::deleteDB(QTcpSocket* socket)
{




}
