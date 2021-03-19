#include "projectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>
#include <iostream>

ProjectScene::ProjectScene() 
{
	m_manager = new QNetworkAccessManager(this);
	m_url = QUrl("http://127.0.0.1:5000/");
	connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(fullUpdate()));
	m_timer->start(2000);
}
ProjectScene::~ProjectScene() 
{
       delete m_manager;	
} 

//sends the data about the object that was on the scene to the server. 
void ProjectScene::sceneChanged(const QList<QRectF> &region) 
{

	QUrlQuery params;
//	params.addQueryItem(tr("id"), tr("?"));

        QList<QGraphicsItem*> changed_items;
        for (QRectF r : region) {
          changed_items = items(r);
        }

    for (QGraphicsItem* i : changed_items) {
        params.addQueryItem(tr("shape"), i->data(1).toString());
	if(i->data(1).toString() == "circle") {

		QGraphicsEllipseItem* c = (QGraphicsEllipseItem*)i;
    		params.addQueryItem(tr("radius"), QString::number(c->rect().x()/2));
  		params.addQueryItem(tr("x"), QString::number(c->rect().x()));
        	params.addQueryItem(tr("y"), QString::number(c->rect().y()));

	} else if (i->data(1).toString() == "line") {

		QGraphicsLineItem* l = (QGraphicsLineItem*)i;
		params.addQueryItem(tr("x1"), QString::number(l->line().x1()));
		params.addQueryItem(tr("y2"), QString::number(l->line().y1()));
		params.addQueryItem(tr("x2"), QString::number(l->line().x2()));
		params.addQueryItem(tr("y2"), QString::number(l->line().x2()));

	} else if (i->data(1).toString() == "rect") {

		QGraphicsRectItem* r = (QGraphicsRectItem*)i;
		params.addQueryItem(tr("x"), QString::number(r->rect().x()));
		params.addQueryItem(tr("y"), QString::number(r->rect().y()));
		params.addQueryItem(tr("width"), QString::number(r->rect().width()));
		params.addQueryItem(tr("height"), QString::number(r->rect().height()));
	}
    }
    	QUrl url(m_url);
	url.setQuery(params.query());
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_manager->get(request);
	

}

void ProjectScene::replyFinished(QNetworkReply* response)
{

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
