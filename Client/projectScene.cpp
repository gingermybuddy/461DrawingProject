#include "projectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>
#include <QPen>
#include <iostream>

ProjectScene::ProjectScene() 
{
	m_manager = new QNetworkAccessManager(this);
	m_url = QUrl("http://127.0.0.1:5000/");
	connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(fullUpdate()));
	m_timer->start(2000);

	setSceneRect(0, 0, 800, 800);
}
ProjectScene::~ProjectScene() 
{
       delete m_manager;	
} 

//sends the data about the object that was on the scene to the server. 
void ProjectScene::sceneChanged(const QList<QRectF> &region) 
{

	QUrlQuery params;

    // get a list of the items we need to update?
    QList<QGraphicsItem*> changed_items;
    for (QRectF r : region) {
      changed_items = items(r);
    }

    for (QGraphicsItem* i : changed_items) {

        if(i->data(0) == -1) {
            params.addQueryItem(tr("id"), tr("none")); //Checks if it has an ID sig
        } else {
            params.addQueryItem(tr("id"), QString::number(i->data(0).toInt()));

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

            params.addQueryItem(tr("radius"), QString::number(c->rect().x()/2));
            params.addQueryItem(tr("x"), QString::number(c->rect().x()));
            params.addQueryItem(tr("y"), QString::number(c->rect().y()));
            params.addQueryItem(tr("color"),  color);

        } else if (i->data(1).toString() == "line") {

            QGraphicsLineItem* l = (QGraphicsLineItem*)i;

            QString color = l->pen().color().name();

            params.addQueryItem(tr("x1"), QString::number(l->line().x1()));
            params.addQueryItem(tr("y2"), QString::number(l->line().y1()));
            params.addQueryItem(tr("x2"), QString::number(l->line().x2()));
            params.addQueryItem(tr("y2"), QString::number(l->line().x2()));
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
	url.setPath(tr("/fullUpdate"));
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
        std::cout << "reply " << std::endl;
}

void ProjectScene::fullUpdate()
{
	std::cout << "Sending request... (2 second timer)" << std::endl;
	QUrl url(m_url);
	url.setPath(tr("/fullUpdate"));
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkReply* data = m_manager->get(request);

}
