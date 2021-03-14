#include "projectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>

ProjectScene::ProjectScene() { }

ProjectScene::~ProjectScene() { } 

//sends the data about the object that was on the scene to the server. 
void ProjectScene::sceneChanged(const
		QList<QRectF> &region) {

	
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QUrl url("http://127.0.0.1:5000/");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QUrlQuery params;
//	params.addQueryItem(tr("id"), tr("?"));

        QList<QGraphicsItem*> changed_items;
        for (QRectF r : region) {
          changed_items = items(r);
        }

    for (QGraphicsItem* i : changed_items) {
        params.addQueryItem(tr("shape"), i->data(1).toString());
    	params.addQueryItem(tr("radius"), QString::number(i->scenePos().x()/2));
  	params.addQueryItem(tr("x"), QString::number(i->scenePos().x()));
        params.addQueryItem(tr("y"), QString::number(i->scenePos().y()));
    }


	connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

	manager->post(request, params.query(QUrl::FullyEncoded).toUtf8());
	

}

