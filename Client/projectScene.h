#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPoint>


#ifndef __PROJECTSCENE_H
#define __PROJECTSCENE_H

class ProjectScene : public QGraphicsScene
{
	Q_OBJECT
	private:
	public slots:
		void sceneChanged(const QList<QRectF> &region);
	signals:	
	public:
		ProjectScene();
		~ProjectScene();
};

#endif
