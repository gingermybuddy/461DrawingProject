#include "projectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>

ProjectScene::ProjectScene() 
{
	m_manager = new QNetworkAccessManager(this);
	m_url = QUrl("http://127.0.0.1:5000/");
	connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

}
ProjectScene::~ProjectScene() 
{
       delete m_manager;	
} 

//sends the data about the object that was on the scene to the server. 
void ProjectScene::sceneChanged(const QList<QRectF> &region) 
{
	QNetworkRequest request(m_url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

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
    		params.addQueryItem(tr("radius"), QString::number(c->scenePos().x()/2));
  		params.addQueryItem(tr("x"), QString::number(c->scenePos().x()));
        	params.addQueryItem(tr("y"), QString::number(c->scenePos().y()));

	} else if (i->data(1).toString() == "line") {

		QGraphicsLineItem* l = (QGraphicsLineItem*)i;
		params.addQueryItem(tr("x1"), QString::number(l->line().x1()));
		params.addQueryItem(tr("y2"), QString::number(l->line().y1()));
		params.addQueryItem(tr("x2"), QString::number(l->line().x2()));
		params.addQueryItem(tr("y2"), QString::number(l->line().x2()));

	} else if (i->data(1).toString() == "rect") {

		QGraphicsRectItem* r = (QGraphicsRectItem*)i;
		params.addQueryItem(tr("x"), QString::number(r->scenePos().x()));
		params.addQueryItem(tr("y"), QString::number(r->scenePos().y()));
		params.addQueryItem(tr("width"), QString::number(r->rect().width()));
		params.addQueryItem(tr("height"), QString::number(r->rect().height()));
	}
    }

	m_manager->post(request, params.query(QUrl::FullyEncoded).toUtf8());
	

}

void ProjectScene::replyFinished(QNetworkReply* response)
{

}

