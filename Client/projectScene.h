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
#include <QTcpSocket>
#include "itemStats.h"

#ifndef __PROJECTSCENE_H
#define __PROJECTSCENE_H

class ProjectScene : public QGraphicsScene
{
	Q_OBJECT
	private:
		QTcpSocket* m_socket;
		std::vector<itemStats> m_tracked_items;
	public slots:
		void sceneChanged(const QList<QRectF> &region);
		void replyFinished(QNetworkReply* response);
		void fullUpdate();

		void readSocket();
		void disconnect();
	signals:	
	public:
		ProjectScene();
		~ProjectScene();
};

#endif
