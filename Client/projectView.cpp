#include "projectView.h"
#include "qevent.h"
#include <QPointF>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <iostream>

ProjectView::ProjectView() : QGraphicsView(), m_tool{0}
{
	setSceneRect(0,0,800,800);
}

ProjectView::~ProjectView()
{
}
void ProjectView::change_tool(int tool)
{
	m_tool = tool;
}
void ProjectView::change_color(int r, int g, int b)
{
	m_color_r = r;
	m_color_g = g;
	m_color_b = b;
}
void ProjectView::fill()
{

        QList<QGraphicsItem*> selected = scene()->selectedItems();
        for(QGraphicsItem* i : selected){
            if(i->data(1).toString() == "line") continue;

            //QBrush brush((QColor(m_color_r, m_color_g, m_color_b), Qt::SolidPattern));
            //brush = i->brush().color();
            QBrush br(QColor(m_color_r, m_color_g, m_color_b), Qt::SolidPattern);

            if(i->data(1).toString() == "rect") {
                QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem *>(i);
                //QGraphicsEllipseItem *circle = qgraphicsitem_cast<QGraphicsEllipseItem *>(circle);
                rect->setBrush(br);
                //circle->setBrush(br);
                rect->update();
            } else if (i->data(1).toString() == "ellipse") {
                QGraphicsEllipseItem* e = (QGraphicsEllipseItem*)i;
                e->setBrush(br);
                e->update();
            }

     }
}
void text(){
	QGraphicsTextItem* text = newGraphicsTextItem("text");
	text->setTextInteractionFlags(Qt::TextEditorInteraction);
	text->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | ti->flags());
	scene()->addItem(text);
}
void ProjectView::circle_tool(qreal x, qreal y, qreal x2, qreal y2)
{		
	QPen pen(QColor(m_color_r, m_color_g, m_color_b)); //Sets up a basic pen
	pen.setWidth(2);

	//Draws a circle where the coordinates were
    QRectF circ(x, y, x2-x, y2-y);
    QGraphicsEllipseItem* r = scene()->addEllipse(circ, pen, QBrush(Qt::transparent));
	r->setFlag(QGraphicsItem::ItemIsSelectable, true);
	r->setFlag(QGraphicsItem::ItemIsMovable, true);
	r->setCursor(Qt::PointingHandCursor);
	r->setData(0, -1);
    r->setData(1, "ellipse");
	//This is some extra data we're adding to the item. Ideally the 'id' parameter
	//iterates, but that hasn't been implemented yet. Probably track how many items are on the scene.
	//The 'setData' function takes in an int as a key and a QVariant (any sort of variable) as parameters.
	//In this case, we're using '0' as the key for the item's id.
	//'1' is the key for the item's type.
}

void ProjectView::line_tool(qreal x, qreal y, qreal x2, qreal y2)
{		
	QPen pen(QColor(m_color_r, m_color_g, m_color_b)); //Sets up a basic pen
	pen.setWidth(2);

    QLineF liner(x, y, x2, y2);
    // std::cout << "Making a line! \n";
    QGraphicsLineItem* line = scene()->addLine(liner, pen);
	line->setFlag(QGraphicsItem::ItemIsSelectable, true);
	line->setFlag(QGraphicsItem::ItemIsMovable, true);
	line->setCursor(Qt::PointingHandCursor);
	line->setData(0, -1);
    line->setData(1, "line");
}

void ProjectView::rect_tool(qreal x, qreal y, qreal x2, qreal y2)
{

	QPen pen(QColor(m_color_r, m_color_g, m_color_b)); //Sets up a basic pen
    pen.setWidth(2);

    QRectF rect(x, y, x2-x, y2-y);
    QGraphicsRectItem* r = scene()->addRect(rect, pen, QBrush(Qt::transparent));
	r->setFlag(QGraphicsItem::ItemIsSelectable, true);
	r->setFlag(QGraphicsItem::ItemIsMovable, true);
	r->setCursor(Qt::PointingHandCursor);
	r->setData(0,-1);
	r->setData(1, "rect");
}

void ProjectView::mousePressEvent(QMouseEvent *event)
{
    QPoint temp = event->pos();
	firstClick = mapToScene(temp);
    // std::cout << "Mouse pressed!\n";
}

void ProjectView::mouseReleaseEvent(QMouseEvent *event)
{
	//Grabs some useful information based on where the mouse was clicked.
    // std::cout << "Mouse released!\n";
	QPoint end = event->pos();
	QPointF q = mapToScene(end);
	qreal x = q.x();
	qreal y = q.y(); 
	
	qreal x2 = firstClick.x();
	qreal y2 = firstClick.y(); 

    switch(m_tool){
	case 0:
	    break; //Default selection tool that intentionally does literally nothing
    case 1:
        line_tool(x, y, x2, y2);
        break;
    case 2:
        circle_tool(x, y, x2, y2);
        break;
    case 3:
         rect_tool(x2, y2, x, y);
         break;
    case 4:
        fill();
        break;
    case 5:
	text();
	break;
    default:
        std::cout << "error" << std::endl;

    }

	//TO-DO LIST:
	//Implement some sort of class that tracks what tool is being used and uses it on a mousePressEvent
	//Create functions based on buttons for a selected tool (e.g. move the rectangle code to the 'Rectangle' button code)
	//Set up color selection so it takes a color parameter (or takes whatever color is currently selected according to some var in ProjectView)

}
