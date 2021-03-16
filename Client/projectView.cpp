#include "projectView.h"
#include <QPointF>
#include <QGraphicsItem>
#include <iostream>

ProjectView::ProjectView() : QGraphicsView(), m_tool{0}
{}

ProjectView::~ProjectView()
{
}
void ProjectView::change_tool(int tool)
{
	m_tool = tool;
}

void ProjectView::circle_tool(qreal x, qreal y)
{		
	QPen pen(Qt::black); //Sets up a basic pen
	pen.setWidth(2);

	//Draws a circle where the coordinates were
    	QRectF rect(x-40, y-40, 120, 120);
    	QGraphicsEllipseItem* r = scene()->addEllipse(rect, pen, QBrush(Qt::transparent));
	r->setFlag(QGraphicsItem::ItemIsSelectable, true);
	r->setFlag(QGraphicsItem::ItemIsMovable, true);
	r->setCursor(Qt::PointingHandCursor);
	r->setData(0, 0);
    	r->setData(1, "circle");
	//This is some extra data we're adding to the item. Ideally the 'id' parameter
	//iterates, but that hasn't been implemented yet. Probably track how many items are on the scene.
	//The 'setData' function takes in an int as a key and a QVariant (any sort of variable) as parameters.
	//In this case, we're using '0' as the key for the item's id.
	//'1' is the key for the item's type.
}

void ProjectView::line_tool(qreal x, qreal y)
{		
	QPen pen(Qt::red); //Sets up a basic pen
	pen.setWidth(2);

    	QLineF liner(x, y, x+100, y+100);
    	QGraphicsLineItem* line = scene()->addLine(liner, pen);
	line->setFlag(QGraphicsItem::ItemIsSelectable, true);
	line->setFlag(QGraphicsItem::ItemIsMovable, true);
	line->setCursor(Qt::PointingHandCursor);
	line->setData(0, 0);
    	line->setData(1, "line");
}

void ProjectView::rect_tool(qreal x, qreal y){
	
	QPen pen(Qt::black);
	pen.setWidth(2);

	QRectF rect(x-60, y-40, 120, 80);
	QGraphicsRectItem* r = scene()->addRect(rect, pen, QBrush(Qt::transparent));
	r->setFlag(QGraphicsItem::ItemIsSelectable, true);
	r->setFlag(QGraphicsItem::ItemIsMovable, true);
	r->setCursor(Qt::PointingHandCursor);
	r->setData(0,0);
	r->setData(1, "rect");
}

void ProjectView::mousePressEvent(QMouseEvent *event)
{
	//Grabs some useful information based on where the mouse was clicked.
	QPoint start = event->pos();
	QPointF q = mapToScene(start);
	qreal x = q.x();
	qreal y = q.y(); 
	
    switch(m_tool){
	case 0:
	    break; //Default selection tool that intentionally does literally nothing
        case 1:
            line_tool(x, y);
            break;
        case 2:
            circle_tool(x, y);
            break;
	case 3:
	    rect_tool(x, y);
	    break;
        default:
            std::cout << "error" << std::endl;

    }

	//TO-DO LIST:
	//Implement some sort of class that tracks what tool is being used and uses it on a mousePressEvent
	//Create functions based on buttons for a selected tool (e.g. move the rectangle code to the 'Rectangle' button code)
	//Set up color selection so it takes a color parameter (or takes whatever color is currently selected according to some var in ProjectView)

}
