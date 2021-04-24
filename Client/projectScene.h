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
#include <QHostAddress>
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
		QTimer* m_timer;

	public slots:
		void sceneChanged(const QList<QRectF> &region);
		void checkPos();
		void readSocket();
		void disconnect();

    signals:
	public:
        int trackItem(QGraphicsItem* item);
        void updateCanvas(std::vector<QJsonObject> objects);
        void sendItem(itemStats package);
        bool connectToBoard(QHostAddress ip, int port, std::string board_id);

		ProjectScene();
		~ProjectScene();
};

#endif
