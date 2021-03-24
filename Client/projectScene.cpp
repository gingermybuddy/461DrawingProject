#include "projectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>
#include <QPen>
#include <iostream>
#include <cmath>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

ProjectScene::ProjectScene() 
{
	m_manager = new QNetworkAccessManager(this);
	m_url = QUrl("http://127.0.0.1:5000/");
	connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    QUrl url(m_url);
    url.setPath(tr("/createBoard"));
    QUrlQuery params;
    params.addQueryItem(tr("bid"), tr("None"));
    url.setQuery(params);
    QNetworkRequest request(url);
    m_manager->get(request);

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(fullUpdate()));
    m_timer->start(2000);


	setSceneRect(0, 0, 800, 800);
}
ProjectScene::~ProjectScene() 
{
       delete m_manager;	
} 

int ProjectScene::trackItem(QGraphicsItem* item)
{
    QString type = item->data(1).toString();
    itemStats tracker;
    tracker.id = m_tracked_items.size();
    item->data(0) = tracker.id;
    tracker.type = type.toStdString();
    if(type != "line") {
        QGraphicsRectItem* c = (QGraphicsRectItem*)item;
        tracker.x = c->rect().x();
        tracker.y = c->rect().y();
        tracker.height = c->rect().height();
        tracker.width = c->rect().width();
        tracker.rgb = c->pen().color();
    } else {
        QGraphicsLineItem* l = (QGraphicsLineItem*)item;
        tracker.x = l->sceneBoundingRect().x();
        tracker.y = l->sceneBoundingRect().y();
        tracker.height = l->sceneBoundingRect().height();
        tracker.width = l->sceneBoundingRect().width();
        tracker.rgb = l->pen().color();
    }
    m_tracked_items.push_back(tracker);
    std:: cout << "Tracking item " << tracker.id << std::endl;
    return tracker.id;
}

//sends the data about the object that was on the scene to the server. 
void ProjectScene::sceneChanged(const QList<QRectF> &region) 
{

	QUrlQuery params;
    params.addQueryItem(tr("bid"), tr("None"));
    // get a list of the items we need to update?
    QList<QGraphicsItem*> changed_items;
    for (QRectF r : region) {
      changed_items = items(r);
    }

    for (QGraphicsItem* i : changed_items) {

        if(i->data(0) == -1) {
            params.addQueryItem(tr("sid"), QString::number(trackItem(i))); //Checks if it has an ID sig
        } else {
            params.addQueryItem(tr("sid"), QString::number(i->data(0).toInt()));

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

        params.addQueryItem(tr("shape"), i->data(1).toString());
        if(i->data(1).toString() == "circle") {

            QGraphicsEllipseItem* c = (QGraphicsEllipseItem*)i;

            QString color = c->pen().color().name();

            params.addQueryItem(tr("radius"), QString::number(std::abs(c->rect().width()/2)));
            params.addQueryItem(tr("x"), QString::number(c->rect().x()));
            params.addQueryItem(tr("y"), QString::number(c->rect().y()));
            params.addQueryItem(tr("color"),  color);

        } else if (i->data(1).toString() == "line") {

            QGraphicsLineItem* l = (QGraphicsLineItem*)i;

            QString color = l->pen().color().name();

            params.addQueryItem(tr("x1"), QString::number(l->line().x1()));
            params.addQueryItem(tr("y2"), QString::number(l->line().y1()));
            params.addQueryItem(tr("x2"), QString::number(l->line().x2()));
            params.addQueryItem(tr("y2"), QString::number(l->line().y2()));
            params.addQueryItem(tr("color"),  color);

        } else if (i->data(1).toString() == "rect") {

            QGraphicsRectItem* r = (QGraphicsRectItem*) i;

            QString color = r->pen().color().name();

            params.addQueryItem(tr("x"), QString::number(r->rect().x()));
            params.addQueryItem(tr("y"), QString::number(r->rect().y()));
            params.addQueryItem(tr("width"), QString::number(r->rect().width()));
            params.addQueryItem(tr("height"), QString::number(r->rect().height()));
            params.addQueryItem(tr("color"),  color);

        }
    }

    QUrl url(m_url);
    url.setPath(tr("/addShape"));
	url.setQuery(params.query());
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_manager->get(request);

	//After this it needs to look at the json file it gets back, and if it got a fresh ID it needs to throw it into the tracked items vector
}

void ProjectScene::replyFinished(QNetworkReply* response)
{
	//QJsonArray json_array = json.response.array();

	QByteArray reply = response->readAll();
    std::cout << "reply received" << std::endl;
	//std::cout << "Data: " << reply.toStdString() << std::endl;
	QString data(reply);
	QJsonObject obj;
	QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
	obj = doc.object();

	for(QString key : obj.keys()) {
		if(key == "data" || key == "shape") return;
		std::cout << "Key: " << key.toStdString() << "Val: " << obj.value(key).toString().toStdString() << std::endl;
	}


}

void ProjectScene::fullUpdate()
{
	std::cout << "Sending request... (2 second timer)" << std::endl;
	QUrl url(m_url);
	url.setPath(tr("/fullUpdate"));
	QNetworkRequest request(url);
    QUrlQuery params;
    params.addQueryItem(tr("bid"), tr("None"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    m_manager->get(request);

}
