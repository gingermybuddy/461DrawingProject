#include <QGraphicsView>
#include <QAction>
#include <QMouseEvent>

#ifndef __PROJECTVIEW_H
#define __PROJECTVIEW_H

class ProjectView : public QGraphicsView
{
	Q_OBJECT
	private:
		int m_tool;
	private slots:
		void change_tool(int tool);
	public:
		void mousePressEvent(QMouseEvent* event);
		ProjectView();
		~ProjectView();
};
#endif
