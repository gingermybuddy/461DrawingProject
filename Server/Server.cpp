#include "Server.h"
#include <iostream>
#include <QDataStream>
#include <QtSql>
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

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //either connect to the database named CMSC461.db or create it if it doesn't exist
    db.setDatabaseName("CMSC461.db");
    db.open();
    QSqlQuery dbQuery;
    dbQuery.exec("CREATE TABLE Ellipse (bid string, sid int, x1 int, x2 int, y1 int, y2 int, fill string, outline string, cid int);");
    dbQuery.exec("CREATE TABLE Line (bid string, sid int, x1 int, x2 int, y1 int, y2 int, fill string, outline string, cid int);");
    dbQuery.exec("CREATE TABLE Rect (bid string, sid int, x int, y int, width int, height int, fill string, outline string, cid int);");
    dbQuery.exec("CREATE TABLE Latex (bid string, sid int, x int, y int, code string, color string, cid int;");
    dbQuery.exec("CREATE TABLE Text (bid string, sid int, x int, y int, code string, color string, cid int;");
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
    m_shapes.push_back(obj);
    std::cout << m_shapes.size() << std::endl;

    QSqlQuery inserter;
	QJsonValue type = obj.value("shape");
	QJsonObject dval  = obj.value("data").toObject();
	if(type.toString() == "ellipse") {
            inserter.prepare("INSERT INTO Ellipse(bid, sid, x1, x2, y1, y2, fill, outline, cid) VALUES(:bid, :sid, :x1, :x2, :y1, :y2, :fill, :outline, :cid);");
    		inserter.bindValue(":bid", dval.value("bid").toString());
            inserter.bindValue(":sid", dval.value("sid").toInt());
    		inserter.bindValue(":x1", dval.value("start").toObject().value("x").toInt());
    		inserter.bindValue(":x2", dval.value("end").toObject().value("x").toInt());
    		inserter.bindValue(":y1", dval.value("start").toObject().value("y").toInt());
    		inserter.bindValue(":y2", dval.value("end").toObject().value("y").toInt());
            inserter.bindValue(":fill", dval.value("fill_color").toString());
            inserter.bindValue(":outline", dval.value("outline_color").toString());
    		inserter.bindValue(":cid", socket->socketDescriptor());
            inserter.exec();

            std::cout << "Executed: " << inserter.executedQuery().toStdString() <<  std::endl;
    		std::cout << "Errors: " << inserter.lastError().text().toStdString() << std::endl;
	
	} else if (type.toString() == "line") {
            inserter.prepare("INSERT INTO Line(bid, sid, x1, x2, y1, y2, outline, cid) VALUES(:bid, :sid, :x1, :x2, :y1, :y2, :outline, :cid);");
    		inserter.bindValue(":bid", dval.value("bid").toString());
            inserter.bindValue(":sid", dval.value("sid").toInt());
            inserter.bindValue(":x1", dval.value("start").toObject().value("x").toInt());
    		inserter.bindValue(":x2", dval.value("end").toObject().value("x").toInt());
    		inserter.bindValue(":y1", dval.value("start").toObject().value("y").toInt());
    		inserter.bindValue(":y2", dval.value("end").toObject().value("y").toInt());
            inserter.bindValue(":outline", dval.value("outline_color").toString());
    		inserter.bindValue(":cid", socket->socketDescriptor());
            inserter.exec();
    		std::cout << "Executed: " << inserter.executedQuery().toStdString() << std::endl;
    		std::cout << "Errors: " << inserter.lastError().text().toStdString() << std::endl;
	
	} else if (type.toString() == "rect") {
            inserter.prepare("INSERT INTO Rect(bid, sid, x, y, width, height, fill, outline, cid) VALUES(:bid, :sid, :x1, :x2, :y1, :y2, :fill, :outline, :cid);");
    		inserter.bindValue(":bid", dval.value("bid").toString());
            inserter.bindValue(":sid", dval.value("sid").toInt());
            inserter.bindValue(":x", dval.value("start").toObject().value("x").toInt());
    		inserter.bindValue(":width", dval.value("end").toObject().value("x").toInt());
    		inserter.bindValue(":y", dval.value("start").toObject().value("y").toInt());
    		inserter.bindValue(":height", dval.value("end").toObject().value("y").toInt());
            inserter.bindValue(":fill", dval.value("fill_color").toString());
            inserter.bindValue(":outline", dval.value("outline_color").toString());
    		inserter.bindValue(":cid", socket->socketDescriptor());
            inserter.exec();
    		std::cout << "Executed: " << inserter.executedQuery().toStdString() << std::endl;
    		std::cout << "Errors: " << inserter.lastError().text().toStdString() << std::endl;
	
	} else if (type.toString() == "text") {
            inserter.prepare("INSERT INTO Text(bid, sid, x, y, text, outline, cid) VALUES(:bid, :sid, :x, :y, :text, :outline, :cid);");
    		inserter.bindValue(":bid", dval.value("bid").toString());
            inserter.bindValue(":sid", dval.value("sid").toInt());
            inserter.bindValue(":x", dval.value("start").toObject().value("x").toInt());
    		inserter.bindValue(":y", dval.value("end").toObject().value("y").toInt());
            inserter.bindValue(":text", dval.value("text").toString());
            inserter.bindValue(":outline", dval.value("outline_color").toString());
    		inserter.bindValue(":cid", socket->socketDescriptor());
            inserter.exec();
    		std::cout << "Executed: " << inserter.executedQuery().toStdString() << std::endl;
    		std::cout << "Errors: " << inserter.lastError().text().toStdString() << std::endl;
    		
	} else if (type.toString() == "latex") {
        	inserter.prepare("INSERT INTO Latex(bid, sid, x, y, code, color, cid) VALUES(:bid, :sid, :x, :y, :text, :color, :cid)");
    		inserter.bindValue(":bid", dval.value("bid").toString());
            inserter.bindValue(":sid", dval.value("sid").toInt());
            inserter.bindValue(":x", dval.value("start").toObject().value("x").toInt());
    		inserter.bindValue(":y", dval.value("end").toObject().value("y").toInt());
            inserter.bindValue(":text", dval.value("text").toString());
            inserter.bindValue(":color", dval.value("color").toString());
    		inserter.bindValue(":cid", socket->socketDescriptor());
            inserter.exec();
    		std::cout << "Executed: " << inserter.executedQuery().toStdString() << std::endl;
    		std::cout << "Errors: " << inserter.lastError().text().toStdString() << std::endl;
	} else {
		std::cout << "Something went wrong." << std::endl;
	}
}

void Server::fullUpdate(QString databasename, QTcpSocket* socket)
{
    //Initialize QSqlQuery for the Circles table of "db"
    QSqlQuery *circle_query = new QSqlQuery;
    circle_query->exec("SELECT * FROM Ellipse");
    //circle_query->first();
    std::vector<itemStats> shapes;
    //Loop through and repeat for whole table
    while(circle_query->next()){
        std::string bid = circle_query->value(0).toString().toStdString();
		std::string shape = "ellipse";
		int sid = circle_query->value(1).toInt();
		double x1 = circle_query->value(2).toDouble();
        double x2 = circle_query->value(4).toDouble();
        double y1 = circle_query->value(3).toDouble();
		double y2 = circle_query->value(5).toDouble();
		QColor fillColor = QColor(circle_query->value(6).toString());
		QColor outlineColor = QColor(circle_query->value(7).toString());
		itemStats temp(bid, shape, sid, x1, y1, x2, y2, fillColor, outlineColor);
        shapes.push_back(temp);
	}

   QSqlQuery *rect_query = new QSqlQuery;
   rect_query->exec("SELECT * FROM Rect");
   //rect_query->first();

	while(rect_query->next()){
        std::string bid = rect_query->value(0).toString().toStdString();
		std::string shape = "rect";
		int sid = rect_query->value(1).toInt();
		double x1 = rect_query->value(2).toDouble();
        double x2 = rect_query->value(4).toDouble();
        double y1 = rect_query->value(3).toDouble();
		double y2 = rect_query->value(5).toDouble();
		QColor fillColor = QColor(circle_query->value(6).toString());
		QColor outlineColor = QColor(circle_query->value(7).toString());
		itemStats temp(bid, shape, sid, x1, y1, x2, y2, fillColor, outlineColor);
        shapes.push_back(temp);
	}

    QSqlQuery *line_query = new QSqlQuery;
	line_query->exec("SELECT * FROM Line");
    line_query->first();
    
	while(line_query->next()){
        std::string bid = line_query->value(0).toString().toStdString();
		std::string shape = "line";
		int sid = line_query->value(1).toInt();
		double x1 = line_query->value(2).toDouble();
		double x2 = line_query->value(3).toDouble();
		double y1 = line_query->value(4).toDouble();
		double y2 = line_query->value(5).toDouble();
		QColor outlineColor = QColor(circle_query->value(6).toString());
		itemStats temp(bid, shape, sid, x1, y1, x2, y2, outlineColor);
        shapes.push_back(temp);
	}
/*
    QSqlQuery *text_query = new QSqlQuery;
	text_query->exec("SELECT * FROM Text");
    text_query->first();
    
	while(text_query->next()){
        std::string bid = text_query->value(0).toString().toStdString();
		std::string shape = "text";
		int sid = text_query->value(1).toInt();
		double x = text_query->value(2).toDouble();
		double y = text_query->value(3).toDouble();
        std::string text = text_query->value(4).toString().toStdString();
		QColor color = QColor(text_query->value(5).toString());
        itemStats temp(bid, shape, sid, x, y, text, color);
        shapes.push_back(temp);
	}
	
    QSqlQuery *latex_query = new QSqlQuery;
	latex_query->exec("SELECT * FROM Latex");
    latex_query->first();
    
	while(latex_query->next()){
        std::string bid = latex_query->value(0).toString().toStdString();
		std::string shape = "latex";
		int sid = latex_query->value(1).toInt();
		double x = latex_query->value(2).toDouble();
		double y = latex_query->value(3).toDouble();
        std::string text = latex_query->value(4).toString().toStdString();
		QColor color = QColor(latex_query->value(5).toString());
        itemStats temp(bid, shape, sid, x, y, text, color);
        shapes.push_back(temp);
    }*/

	//Create a JSON object of all the shapes using their sid as a key
    delete rect_query;
    delete line_query;
    delete circle_query;

    QJsonObject full_board;
    full_board.insert("fullUpdate", "test");

    /*for(itemStats temp : shapes){
        std::cout << "shape ";
        std::cout << temp.type;
        std::cout << " added" << std::endl;
        full_board.insert(QString::number(temp.id), temp.toJson());
    }*/

    std::cout << m_shapes.size() << std::endl;
    for (QJsonObject j : m_shapes) {
        std::cout << QJsonDocument(j).toJson(QJsonDocument::Compact).toStdString() << std::endl;
        QJsonObject data = j.value("data").toObject();
        int id = data.value("sid").toInt();
        std::cout << id << std::endl;
        full_board.insert(QString::number(id), j);
    }

	//Create JSON Document to write to the buffer
	QJsonDocument doc(full_board);

	//Write our JSON object to the socket
	QByteArray buf = doc.toJson();
	std::cout << "Sending: " << buf.toStdString() << std::endl;
	QDataStream sockstream(socket);
	sockstream.startTransaction();
    sockstream << buf;
}

void Server::createBoard(QTcpSocket* socket)
{
    //Create a QSqlDatabase named db
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //either connect to the database named CMSC461.db or create it if it doesn't exist
    db.setDatabaseName("CMSC461.db");
    db.open();
    //Create and QSqlQuery pointer to be used to execute commands
    QSqlQuery* dbQuery = new QSqlQuery;



    //bid = Board ID
    //sid = Shape ID
    //cid = Client/User ID
    dbQuery->exec("CREATE TABLE Ellipse (bid string, sid int, x1 int, x2 int, y1 int, y2 int, fill string, outline string, cid int);");
    dbQuery->exec("CREATE TABLE Line (bid string, sid int, x1 int, x2 int, y1 int, y2 int, fill string, outline string, cid int);");
    dbQuery->exec("CREATE TABLE Rect (bid string, sid int, x int, y int, width int, height int, fill string, outline string, cid int);");
    dbQuery->exec("CREATE TABLE Latex (bid string, sid int, x int, y int, code string, color string, cid int;");
    dbQuery->exec("CREATE TABLE Text (bid string, sid int, x int, y int, code string, color string, cid int;");

    ownedDB newDB;
    newDB.id = socket->socketDescriptor();
    newDB.db = db;

    databases.push_back(newDB);
	return;
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
    }
    std::remove(name.c_str());
}
