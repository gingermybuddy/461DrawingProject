#include <QGraphicsView>
#include <QAction>
#include <QMouseEvent>
#include <QPoint>

#ifndef __PROJECTVIEW_H
#define __PROJECTVIEW_H

class ProjectView : public QGraphicsView
{
	Q_OBJECT
	private:
		int m_tool;
	private slots:
		void change_tool(int tool);
		void rect_tool(qreal x, qreal y);
	public:
		void mousePressEvent(QMouseEvent* event);
		ProjectView();
		~ProjectView();
};
#endif
