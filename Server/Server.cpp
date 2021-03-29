#include "Server.h"
#include <iostream>
#include <QDataStream>
#include <QtSql>
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
    } else if (buf.toStdString() == "fullUpdate") {
	std::cout << "Full update requested!" << std::endl;
	fullUpdate("CMSC461.db", socket);
	return;
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

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("CMSC461.db");
	db.open();
	QSqlQuery inserter;
	QJsonValue type = obj.value("shape");
	QJsonObject dval  = obj.value("data").toObject();
	if(type.toString() == "ellipse") {
		inserter.prepare("INSERT INTO Ellipse VALUES(?,?,?,?,?,?,?,?)");
	} else if (type.toString() == "line") {	
		inserter.prepare("INSERT INTO Line VALUES(?,?,?,?,?,?,?,?)");
	} else if (type.toString() == "rect") {

		inserter.prepare("INSERT INTO Rect VALUES(?,?,?,?,?,?,?,?)");
	}
	inserter.bindValue(0, dval.value("bid").toString());
	inserter.bindValue(1, dval.value("sid").toString());
	inserter.bindValue(2, dval.value("start").toObject().value("x").toInt());
	inserter.bindValue(3, dval.value("end").toObject().value("x").toInt());
	inserter.bindValue(4, dval.value("start").toObject().value("y").toInt());
	inserter.bindValue(5, dval.value("end").toObject().value("y").toInt());
	inserter.bindValue(6, dval.value("color").toString());
	inserter.bindValue(7, socket->socketDescriptor());
	inserter.exec();


}

void Server::fullUpdate(QString databaseName, QTcpSocket* socket)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);
    db.open();

    //Initialize QSqlQuery for the Circles table of "db"
    QSqlQuery *circle_query = new QSqlQuery;
	circle_query->exec("SELECT * FROM Ellipse");
    circle_query->first();

    /*
    //Set values to pass into itemStats contstructor
    std::string bid = circle_query->value(0).toString().toStdString();
	std::string shape = "ellipse";
	int sid = circle_query->value(1).toInt();
	double x1 = circle_query->value(2).toDouble();
	double x2 = circle_query->value(3).toDouble();
	double y1 = circle_query->value(4).toDouble();
	double y2 = circle_query->value(5).toDouble();
	QColor color = QColor(circle_query->value(6).toString());

    //Construct itemStats and convert to JSON
	itemStats temp(bid, shape, sid, x1, y1, x2, y2, color);
	QJsonObject cir = temp.toJson();
    //Push back into our array of JSON objects
    shapes.push_back(cir);
	*/

    //Loop through and repeat for whole table
    while(circle_query->next()){
        std::string bid = circle_query->value(0).toString().toStdString();
		std::string shape = "ellipse";
		int sid = circle_query->value(1).toInt();
		double x1 = circle_query->value(2).toDouble();
		double x2 = circle_query->value(3).toDouble();
		double y1 = circle_query->value(4).toDouble();
		double y2 = circle_query->value(5).toDouble();
		QColor color = QColor(circle_query->value(6).toString());
		itemStats temp(bid, shape, sid, x1, y1, x2, y2, color);
		QJsonObject cir = temp.toJson();
		shapes.push_back(cir);
	}
	
	//Create a JSON object of all the shapes using their sid as a key
    QJsonObject full_board;
	full_board.insert("fullUpdate", "test");
	for(QJsonObject temp : shapes){
		full_board.insert(temp.value("sid").toString(), temp);
	}
	//Create JSON Document to write to the buffer
	QJsonDocument doc(full_board);

	//Write our JSON object to the socket
	QByteArray buf = doc.toJson();
	std::cout << "Sending: " << buf.toStdString() << std::endl;
	QDataStream sockstream(socket);
	sockstream.startTransaction();
	sockstream >> buf;
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
    dbQuery->exec("CREATE TABLE Rect (bid int, sid int, x1 int, x2 int, y1 int, y2 int, color string, cid int);");

    ownedDB newDB;
    newDB.id = socket->socketDescriptor();
    newDB.db = db;

    databases.push_back(newDB);
}

void Server::deleteDB(QTcpSocket* socket)
{




}
