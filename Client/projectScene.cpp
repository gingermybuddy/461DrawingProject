#include "projectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>
#include <QPen>
#include <iostream>
#include <QHostAddress>

ProjectScene::ProjectScene() 
{
	setSceneRect(0, 0, 800, 800);
        m_socket = new QTcpSocket(this);
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnect()));
        m_socket->connectToHost(QHostAddress::LocalHost, 5000);
        if(m_socket->waitForConnected()){
                std::cout << "Connected." << std::endl;
        }


}
ProjectScene::~ProjectScene() 
{
}

void ProjectScene::readSocket()
{
}

void ProjectScene::disconnect()
{
}


//sends the data about the object that was on the scene to the server. 
void ProjectScene::sceneChanged(const QList<QRectF> &region) 
{

    // get a list of the items we need to update?
    QList<QGraphicsItem*> changed_items;
    for (QRectF r : region) {
      changed_items = items(r);
    }

    for (QGraphicsItem* i : changed_items) {

        if(i->data(0) == -1) {
		//Is this a fresh item?
		std::cout << "Fresh item!" << std::endl;
        } else {
            QRectF chk = i->sceneBoundingRect();
            itemStats checker;
            for(itemStats j : m_tracked_items) {
                if(j.id == i->data(0).toInt()) {
                    checker = j;
                    break;
                } //Finds the item with a matching ID from the internally tracked things.
            }

            //This if statement is going to be nasty. All it's going to do is check the item's current state with its last-known state. It does this by checking every variable in the itemStats class against what it presently is in the scene.
            //It should be accurate to find out which item is changed.
            if((chk.x() == checker.x) && (chk.y() == checker.y) && (chk.height() == checker.height) && (chk.width() == checker.width)) { //If nothing changed... 
                continue;
            }

        }

	QString str = tr("Data would go here, if we HAD any!");
	QByteArray data = str.toUtf8();

	QDataStream socketstream(m_socket);
	socketstream << data;

    }

}

void ProjectScene::replyFinished(QNetworkReply* response)
{
	//QJsonArray json_array = json.response.array();

	QByteArray reply = response->readAll();
        std::cout << "reply " << std::endl;
	std::cout << "Data: " << reply.toStdString() << std::endl;
}

void ProjectScene::fullUpdate()
{

}
