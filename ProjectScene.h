#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPoint>


#ifndef __PROJECTSCENE_H
#define __PROJECTSCENE_H

class ProjectScene : public QGraphicsScene
{
	Q_OBJECT
	private:
	private slots:
	public:
		ProjectScene();
		~ProjectScene();
		void changed(QList<QRectF> &region);
}; //Not much to see here yet, but this is where network code goes

#endif
