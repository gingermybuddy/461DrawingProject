#include "ProjectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>

ProjectScene::ProjectScene() { }

ProjectScene::~ProjectScene() { } void ProjectScene::sceneChanged(const
		QList<QRectF> &region) {

//This is a starter for sending a request to the server. It is not complete
//yet.
	
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QUrl url("http://127.0.0.1:5000/");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QUrlQuery params;
	params.addQueryItem("id", "?");

/*
	QList<QRectF> data =region->items();
	for(QRectF* i : data){
		qreal height = i->boundingRect().height();
		qreal width = i->boundingRect().width(); 
		
		QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem *>(i);
		int x = rect->rect().x();
		int y = rect->rect().y();
	}
*/

	connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

	manager->post(request, params.query(QUrl::FullyEncoded).toUtf8());
	

} //Network code and such goes here!

