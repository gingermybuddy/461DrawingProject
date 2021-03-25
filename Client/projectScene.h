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
#include <string>
#include "itemStats.h"

#ifndef __PROJECTSCENE_H
#define __PROJECTSCENE_H

class ProjectScene : public QGraphicsScene
{
	Q_OBJECT
	private:
		QTcpSocket* m_socket;
		std::vector<itemStats> m_tracked_items;
		std::string m_board_id;

	public slots:
		void sceneChanged(const QList<QRectF> &region);
		void readSocket();
		void disconnect();
	signals:	
	public:
                int trackItem(QGraphicsItem* item);
		ProjectScene();
		~ProjectScene();
};

#endif
