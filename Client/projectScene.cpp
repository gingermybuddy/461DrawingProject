#include "projectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <iostream>
#include <QHostAddress>
#include <cmath>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPen>

ProjectScene::ProjectScene() 
{
	setSceneRect(0, 0, 800, 800);

	//Sets up the socket that will be connected to the server.
        m_socket = new QTcpSocket(this);
	
	//Connects signals from the socket to functions on the client.
	//readyRead signals that the socket has received data and is ready
	//to read it. Disconnect is exactly what it says on the tin.
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnect()));

	//Connects the socket to the host. Host IP and port are currently fixed,
	//but we can set them to variables later.
        m_socket->connectToHost(QHostAddress::LocalHost, 5000);
        if(m_socket->waitForConnected()){
                std::cout << "Connected." << std::endl;
        } else {
		std::cout << "Failed to connect to server." << std::endl;
	}
	//Board ID is also fixed right now.
	m_board_id = "CMSC461";

	QByteArray request;
	QString fup = "fullUpdate";
	QDataStream sockstream(m_socket);
	request = fup.toUtf8();
	sockstream << request;


}
ProjectScene::~ProjectScene() 
{
	delete m_socket;
}

void ProjectScene::readSocket()
{
	//Does exactly what it does on the server end.
	//The QByteArray in question will be received in the form of a JSON object.
	QByteArray buf;
	QDataStream sockstream(m_socket);
	sockstream.startTransaction();
	sockstream >> buf;
	if(!sockstream.commitTransaction()) {
		std::cout << "Awaiting additional data." << std::endl;
		return;
	}
	//Right now all it does is pass received data into a string and outputs it
	//to the console.
	std::cout << "Received " << buf.toStdString() << std::endl;
	//Passes the byte array into some JSON objects that we can use later.
        QJsonDocument doc = QJsonDocument::fromJson(buf);
        QJsonObject obj = doc.object();

	//What the hell kind of JSON is this?
	QJsonValue fup = obj.value("fullUpdate");
	if(fup.toString() == "test") {
		std::cout << "This is a full update; we should parse this." << std::endl;
	}
}

void ProjectScene::disconnect()
{
	//deleteLater MUST be called instead of a regular delete.
	m_socket -> deleteLater();
	m_socket = nullptr;
}


int ProjectScene::trackItem(QGraphicsItem* item)
{

	QString type = item->data(1).toString();
	int id = m_tracked_items.size();
	item->setData(0, id);
	m_tracked_items.push_back(itemStats(m_board_id, item));
	return id;
	//Returns the id of the newly-tracked item.
}

void ProjectScene::fullUpdate(QJsonObject data)
{
	//This should only be called once, right when the scene boots up, to grab a list of items
	//from the server itself. That'll catch it up to speed with everything else.
}


//sends the data about the object that was on the scene to the server. 
void ProjectScene::sceneChanged(const QList<QRectF> &region) 
{

    // get a list of the items we need to update?
    QList<QGraphicsItem*> changed_items;
    for (QRectF r : region) {
      changed_items = items(r);
    }

    //Gets the items in the area that has changed/
    for (QGraphicsItem* i : changed_items) {

        if(i->data(0).toInt() == -1) {
		//Is this a fresh item?
		int new_id = trackItem(i);
		std::cout << "Fresh item! ID: " << new_id << std::endl;
		//Immediately tracks the item if it's fresh
		//Sets the ID as well
	} else {
            itemStats checker;
            for(itemStats j : m_tracked_items) {
                if(j.id == i->data(0).toInt()) {
                    checker = j;
                    break;
                } //Finds the item with matching ID from the internally tracked things.
            }

            //This if statement is going to be nasty. All it's going to do is check the item's current state with its last-known state. It does this by checking every variable in the itemStats class against what it presently is in the scene.
            //It should be accurate to find out which item is changed.
	    if(i->data(0).toString() != "line") {
		    QGraphicsRectItem* r = (QGraphicsRectItem*)i;
		    QRectF chk = r->rect();
		    if((chk.x() == checker.x) && (chk.y() == checker.y) && (chk.height() == checker.height) && (chk.width() == checker.width)) { 
			    //If nothing changed... 
			    //Currently does not check color
			    continue;
            	}
	    } else {
		    QGraphicsLineItem* l = (QGraphicsLineItem*)i;
		    QLineF chk = l->line();
		    if((chk.x1() == checker.x) && (chk.y1() == checker.y) && (chk.y2() == checker.height) && (chk.x2() == checker.width)) { 
			    continue;
	    }
		    //To do:
		    //The above functions also need to check if the color changed.
		    //The above if statements will also need to be updated as we add more variables to the item.
		    //Right now, for whatever reason, the above if statements do not trigger if the item is moved.
		    //Needs to be debugged.

        }
	}

	//So if it's made it to this point, something has changed about
	//the item. It's time to send it out to the server.

	QByteArray data = itemStats(m_board_id, i).byteData();
	//Turns the item into a json, then turns it into a byte array.
	//See 'itemStats.h' for more info on how it does it.

	QDataStream socketstream(m_socket);
	socketstream << data;
    	}
}
