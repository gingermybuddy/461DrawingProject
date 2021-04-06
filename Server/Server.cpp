#include "Server.h"
#include <iostream>
#include <QDataStream>
#include <QSqlQuery>
#include <QString>
#include <stdio.h>
#include <string>

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
    dbQuery->exec("CREATE TABLE Ellipse (bid int, sid int, x1 int, x2 int, y1 int, y2 int, fill string, outline string, cid int);");
    dbQuery->exec("CREATE TABLE Line (bid int, sid int, x1 int, x2 int, y1 int, y2 int, fill string, outline string, cid int);");
    dbQuery->exec("CREATE TABLE Rect (bid int, sid int, x int, y int, width int, height int, fill string, outline string, cid int);");
    dbQuery->exec("CREATE TABLE Latex (bid int, sid int, x int, y int, code string, color string, cid int");
    dbQuery->exec("CREATE TABLE Text (bid int, sid int, x int, y int, code string, color string, cid int");

    ownedDB newDB;
    newDB.id = socket->socketDescriptor();
    newDB.db = db;

    databases.push_back(newDB);
	return;
}

void Server::saveDB(QTcpSocket* socket)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("CMSC461.db");
	db.open();
	QByteArray everything;
	std::string board = "CMSC461";

	QSqlQuery elQuery("SELECT * FROM Ellipse");
	QByteArray ellipses;
	while(elQuery.next()){
		std::string shape = "ellipse";
		QByteArray data = itemStats(board,shape,elQuery.value(1).toInt(),elQuery.value(2).toDouble(),elQuery.value(3).toDouble(),elQuery.value(4).toDouble(),elQuery.value(5).toDouble(),QColor(elQuery.value(6).toString())).byteData();
		QJsonDocument doc = QJsonDocument::fromJson(data);
		//QJsonObject obj = doc.object();
		data = doc.toJson();
		ellipses.append(data);
	}
	everything += "{\"Ellipse\":{";
	everything.append(ellipses);

	QSqlQuery liQuery("SELECT * FROM Line");
	QByteArray lines;
	while(liQuery.next()){
		std::string shape = "line";
		QByteArray data = itemStats(board,shape,liQuery.value(1).toInt(),liQuery.value(2).toDouble(),liQuery.value(3).toDouble(),liQuery.value(4).toDouble(),liQuery.value(5).toDouble(),QColor(liQuery.value(6).toString())).byteData();
		QJsonDocument doc = QJsonDocument::fromJson(data);
		//QJsonObject obj = doc.object();
		data = doc.toJson();
		lines.append(data);
	}
	everything += "}\"Line\":{";
	everything.append(lines);

	QSqlQuery reQuery("SELECT * FROM Rect");
	QByteArray rectangles;
	while(reQuery.next()){
		std::string shape = "rect";
		QByteArray data = itemStats(board,shape,reQuery.value(1).toInt(),reQuery.value(2).toDouble(),reQuery.value(3).toDouble(),reQuery.value(4).toDouble(),reQuery.value(5).toDouble(),QColor(reQuery.value(6).toString())).byteData();
		QJsonDocument doc = QJsonDocument::fromJson(data);
		//QJsonObject obj = doc.object();
		data = doc.toJson();
		rectangles.append(data);
	}
	everything += "}\"Rect\":{";
	everything.append(rectangles);

	everything += "}";
	QJsonDocument doc = QJsonDocument::fromJson(everything);
	//QJsonObject obj = doc.object();
	everything = doc.toJson();
	std::cout << "Client Disconnected. Sending:\n" << everything.toStdString() << std::endl;

	QDataStream socketstream(socket);
	socketstream << everything;
	//cout << "{Ellipse:{" << elQuery.result() << "}Line:{" << liQuery.result() << "}Rect:{" << reQuery.result() << "}";
}

void Server::deleteDB(QTcpSocket* socket)
{
    int id = socket->socketDescriptor();
    std::string name;
    QVector<ownedDB> newVec;

    for(int i = 0; i < databases.size(); ++i)
    {
        if(databases[i].id == id){
            name = databases[i].db.databaseName().toStdString();
            //ownedDB* temp = databases[i];
            //databases.erase(temp);
            break;
        }else{
            newVec.push_back(databases[i]);
        }
        databases = newVec;
    }
    std::remove(name.c_str());
}
