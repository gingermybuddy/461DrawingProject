#include "projectView.h"
#include "qevent.h"
#include "qpixmap.h"
#include <QPointF>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPixmap>
#include <QColor>
#include <iostream>
#include "cmath"
#include "math.h"
#include <QInputDialog>
#include <stdio.h>
#include <stdlib.h>

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
int ProjectView::get_m_tool()
{
    return m_tool;
}
int ProjectView::get_m_color_r()
{
    return m_color_r;
}
int ProjectView::get_m_color_g()
{
    return m_color_g;
}
int ProjectView::get_m_color_b()
{
    return m_color_b;
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
void ProjectView::text_tool(qreal x, qreal y)
{
	bool ok;
    	QString temp = QInputDialog::getText(this, tr("Add Text"), tr("Enter text:"), QLineEdit::Normal, tr("My text"), &ok);
	if(!ok || temp.isEmpty()) return;

	QGraphicsTextItem* text = scene()->addText(temp);
        text->setPos(x, y);
    text->setDefaultTextColor(QColor(m_color_r, m_color_g, m_color_b));
	// text->setTextInteractionFlags(Qt::TextEditorInteraction);
	text->setFlag(QGraphicsItem::ItemIsSelectable, true);
	text->setFlag(QGraphicsItem::ItemIsMovable, true);
	text->setCursor(Qt::PointingHandCursor);
	text->setData(0, -1);
        text->setData(1, "text");
}
void ProjectView::latex_tool(qreal x, qreal y)
{
	bool ok;
    // QString temp = QInputDialog::getText(this, tr("Add Math"), tr("Enter text:"), QLineEdit::Normal, tr("\\left[-\\frac{\\hbar^2}{2m}\\frac{\\partial^2}{\\partial x^2}+V(x)\\right]\\Psi(x)=\\mathrm{i}\\hbar\\frac{\\partial}{\\partial t}\\Psi(x)") , &ok);
    QString temp = QInputDialog::getMultiLineText(this, tr("add Math"), tr("Enter text:"), tr("\\left[-\\frac{\\hbar^2}{2m}\\frac{\\partial^2}{\\partial x^2}+V(x)\\right]\\Psi(x)=\\mathrm{i}\\hbar\\frac{\\partial}{\\partial t}\\Psi(x)"), &ok);
	if(!ok || temp.isEmpty()) return;

    // XXX WILL ONLY WORK ON UNIX XXX
    // TODO
    // create file
    std::string doc = "\\documentclass{standalone}\n\\begin{document}\n\\Huge $" + temp.toStdString() + "$\n\\end{document}";
    system("touch temp.tex");
    // pipe into file
    std::string echo = "echo '" + doc + "' > temp.tex";
    system(echo.c_str());
    // pdflatex
    int isGood = system("pdflatex -interaction=nonstopmode temp.tex");
    std::cout << "RETURNVALUE WAS: "<< isGood << std::endl;

    // convert to a png
    system("pdftoppm -png -r 500 temp.pdf > temp.png");
    // open file in qt
    QPixmap pix(QString("./temp.png"));
    std::cout << pix.isNull() << std::endl;
    // pix = pix.scaled(2 * pix.width(), 2 * pix.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // remove file
    system("rm temp.png");
    system("rm temp.tex");
    system("rm temp.pdf");
    system("rm temp.fls");
    system("rm temp.log");
    system("rm temp.aux");

    if (isGood == 0){
        // The latex was valid!
        std::cout << "WAS GOOD!"<< std::endl;
        QGraphicsPixmapItem* text = scene()->addPixmap(pix);
        text->setPos(x,y);
        text->setFlag(QGraphicsItem::ItemIsSelectable, true);
        text->setFlag(QGraphicsItem::ItemIsMovable, true);
        text->setCursor(Qt::PointingHandCursor);
        text->setData(0, -1);
        text->setData(1, "latex");
        text->setData(2, temp);
    } else {
        // otherwise, it was invalid latex
        std::cout << "WAS NOT GOOD!"<< std::endl;
        QGraphicsTextItem* text = scene()->addText(temp);
        text->setDefaultTextColor(QColor(m_color_r, m_color_g, m_color_b));
        text->setPos(x,y);
        text->setFlag(QGraphicsItem::ItemIsSelectable, true);
        text->setFlag(QGraphicsItem::ItemIsMovable, true);
        text->setCursor(Qt::PointingHandCursor);
        text->setData(0, -1);
        text->setData(1, "text");
    }
	// text->setTextInteractionFlags(Qt::TextEditorInteraction);

}
void ProjectView::arrow_tool(qreal x, qreal y, qreal x2, qreal y2)
{
    QPen pen(QColor(m_color_r, m_color_g, m_color_b)); //Sets up a basic pen
    pen.setWidth(2);

    qreal arrowSize = 20;
    QLineF line(x,y,x2,y2);

    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                            cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                            cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead << line.p1() << arrowP1 << arrowP2;
    QGraphicsLineItem* liner = scene()->addLine(line,pen);
    QGraphicsPolygonItem* head = scene()->addPolygon(arrowHead,pen);
	
	liner->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	liner->setCursor(Qt::PointingHandCursor);
	liner->setData(0, -1);
	liner->setData(1, "arrow");
    head->setData(1, "arrowhead");
    liner->setData(3, (unsigned long long int)head); //You ever feel like you committed a sin?
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
void ProjectView::bezier_tool(qreal x, qreal y, qreal x2, qreal y2)
{		
	QPen pen(QColor(m_color_r, m_color_g, m_color_b)); //Sets up a basic pen
	pen.setWidth(2);

    QPainterPath path;
    path.moveTo(x,y);
    path.quadTo(((x+x2)/2) + 50,((y+y2)/2) + 50,x2,y2);

    QGraphicsPathItem* line = scene()->addPath(path, pen);
    line->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    line->setCursor(Qt::PointingHandCursor);
    line->setData(0, -1);
    line->setData(1, "bezier");

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
        text_tool(x, y);
        break;
    case 6:
        latex_tool(x, y);
        break;
    case 7:
        arrow_tool(x, y, x2, y2);
        break;
    case 8:
        bezier_tool(x, y, x2, y2);
        break;
    default:
        std::cout << "error" << std::endl;
        break;

    }

	//TO-DO LIST:
	//Implement some sort of class that tracks what tool is being used and uses it on a mousePressEvent
	//Create functions based on buttons for a selected tool (e.g. move the rectangle code to the 'Rectangle' button code)
	//Set up color selection so it takes a color parameter (or takes whatever color is currently selected according to some var in ProjectView)

}
