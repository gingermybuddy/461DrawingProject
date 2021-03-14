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

	public:
		void change_tool(int tool);
        void circle_tool(qreal x, qreal y);
        void line_tool(qreal x, qreal y);
		void mousePressEvent(QMouseEvent* event);
		ProjectView();
		~ProjectView();
};
#endif
