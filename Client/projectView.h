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
                //QBrush m_fill;

        QPointF firstClick;
    public slots:
	public:
        int get_m_tool();
        int get_m_color_r();
        int get_m_color_g();
        int get_m_color_b();

	void change_tool(int tool);
	void change_color(int r, int g, int b);
        void fill();
	void text_tool(qreal x, qreal y);
	void latex_tool(qreal x, qreal y);
        void arrow_tool(qreal x, qreal y, qreal x2, qreal y2);
	void circle_tool(qreal x, qreal y,qreal x2, qreal y2);
        void line_tool(qreal x, qreal y, qreal x2, qreal y2);
        void bezier_tool(qreal x, qreal y, qreal x2, qreal y2);
	void rect_tool(qreal x, qreal y, qreal x2, qreal y2);
        void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	ProjectView();
	~ProjectView();
};
#endif
