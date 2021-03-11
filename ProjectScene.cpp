#include "ProjectScene.h"
#include <Qhttp.h>

ProjectScene::ProjectScene() { }

ProjectScene::~ProjectScene() { } void ProjectScene::sceneChanged(const
		QList<QRectF> &region) {

//This is a starter for sending a request to the server. It is not complete
//yet.
	
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
			        this, SLOT(replyFinished(QNetworkReply*)));

	data = region->items();  
	
	//array to add to post request
	//QByteArray data;
	//QUrlQUery params;


	manager->post(QNetworkRequest(QUrl("http://127.0.0.1:5000/")), data);
	

} //Network code and such goes here!

