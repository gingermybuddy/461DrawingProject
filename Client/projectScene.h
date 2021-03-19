#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPoint>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>
#include <QNetworkReply>
#include <QTimer>

#ifndef __PROJECTSCENE_H
#define __PROJECTSCENE_H

class ProjectScene : public QGraphicsScene
{
	Q_OBJECT
	private:
		QNetworkAccessManager* m_manager;
		QUrl m_url; 
		QTimer* m_timer;
	public slots:
		void sceneChanged(const QList<QRectF> &region);
		void replyFinished(QNetworkReply* response);
		void fullUpdate();
	signals:	
	public:
		ProjectScene();
		~ProjectScene();
};

#endif
