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
		std::vector<itemStats> m_tracked_items;
        QTcpSocket* m_socket;

	public slots:
		void sceneChanged(const QList<QRectF> &region);
		void replyFinished(QNetworkReply* response);
		void fullUpdate();
        //SOCKET STUFF
        void socketConnected();
        void socketDisconnected();
        void bytesWritten(qint64 bytes);
        void socketReady();
	signals:	
	public:
		ProjectScene();
		~ProjectScene();
};

#endif
