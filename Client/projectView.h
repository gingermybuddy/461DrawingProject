#include <QGraphicsView>
#include <QAction>
#include <QMouseEvent>
#include <QPoint>
#include <QPointF>

#ifndef __PROJECTVIEW_H
#define __PROJECTVIEW_H

class ProjectView : public QGraphicsView
{
	Q_OBJECT
	private:
		int m_tool;
		int m_color_r;
		int m_color_g;
		int m_color_b;

        QPointF firstClick;
	private slots:

	public:
		void change_tool(int tool);
		void change_color(int r, int g, int b);

        void circle_tool(qreal x, qreal y);
        void line_tool(qreal x, qreal y, qreal x2, qreal y2);
		void rect_tool(qreal x, qreal y, qreal x2, qreal y2);
		void mousePressEvent(QMouseEvent* event);
		void mouseReleaseEvent(QMouseEvent* event);
		ProjectView();
		~ProjectView();
};
#endif
