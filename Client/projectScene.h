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

#ifndef __PROJECTSCENE_H
#define __PROJECTSCENE_H
struct itemStats 
{
	std::string type;
	int id;
	double x;
	double y;
	double height;
	double width;
	QColor rgb;
};

class ProjectScene : public QGraphicsScene
{
	Q_OBJECT
	private:
		QNetworkAccessManager* m_manager;
		QUrl m_url; 
		QTimer* m_timer;
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
