#include "projectScene.h"
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QRectF>
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <iostream>
#include <QHostAddress>
#include <cmath>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPen>

using namespace std;

ProjectScene::ProjectScene() 
{
	setSceneRect(0, 0, 800, 800);

	//Sets up the socket that will be connected to the server.
        m_socket = new QTcpSocket(this);
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(checkPos()));
	m_timer->start(2000);
	
	//Connects signals from the socket to functions on the client.
	//readyRead signals that the socket has received data and is ready
	//to read it. Disconnect is exactly what it says on the tin.
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnect()));

	//Connects the socket to the host. Host IP and port are currently fixed,
	//but we can set them to variables later.
        m_socket->connectToHost(QHostAddress::LocalHost, 5000);
        if(m_socket->waitForConnected()){
                std::cout << "Connected." << std::endl;
        } else {
		std::cout << "Failed to connect to server." << std::endl;
	}
	//Board ID is also fixed right now.
	m_board_id = "CMSC461";

	QByteArray request;
	QString fup = "fullUpdate";
	QDataStream sockstream(m_socket);
	request = fup.toUtf8();
	sockstream << request;


}
ProjectScene::~ProjectScene() 
{
	delete m_socket;
}

void ProjectScene::readSocket()
{
	//Does exactly what it does on the server end.
	//The QByteArray in question will be received in the form of a JSON object.
	QByteArray buf;
	QDataStream sockstream(m_socket);
	sockstream.startTransaction();
	sockstream >> buf;
	if(!sockstream.commitTransaction()) {
		std::cout << "Awaiting additional data." << std::endl;
		return;
	}
	//Right now all it does is pass received data into a string and outputs it
	//to the console.
    std::cout << "Received " <<  QJsonDocument::fromJson(buf).toJson(QJsonDocument::Compact).toStdString() << std::endl;
	//Passes the byte array into some JSON objects that we can use later.
        QJsonDocument doc = QJsonDocument::fromJson(buf);
        QJsonObject obj = doc.object();

	//What the hell kind of JSON is this?
	QJsonValue fup = obj.value("fullUpdate");
	if(fup.toString() == "test") {
		std::cout << "This is a full update; we should parse this." << std::endl;
        obj.remove("fullUpdate");
        std::vector<QJsonObject> shapes;
        foreach(QString str, obj.keys()) {
            shapes.push_back(obj.value(str).toObject());
        }
        updateCanvas(shapes);
        for(QGraphicsItem* i : items()){
            m_tracked_items.push_back(itemStats(m_board_id, i));
        }
		return;
    }
    //If it's not a full update it's just some kind of other shape update
    //Either a new shape got added or an existing one is updated.
    std::vector<QJsonObject> shapes;
    shapes.push_back(obj);
    updateCanvas(shapes);
    return;

}

void ProjectScene::disconnect()
{
	//deleteLater MUST be called instead of a regular delete.
	m_socket -> deleteLater();
	m_socket = nullptr;
}

void ProjectScene::updateCanvas(std::vector<QJsonObject> objects)
{
    for(QJsonObject obj : objects) {
        QJsonObject d = obj.value("data").toObject();
        std::string type = obj.value("shape").toString().toStdString();
        QJsonObject start = d.value("start").toObject();

        QJsonObject scenepos = d.value("scenepos").toObject();
        qreal newx = scenepos.value("scenex").toDouble();
        qreal newy = scenepos.value("sceney").toDouble();
        bool found = false;
        for(QGraphicsItem* i : items()) {
            if(obj.value("data").toObject().value("sid").toInt() == i->data(0).toInt()) {
                if (i->data(1) == "arrow") {
                    QGraphicsPolygonItem* header = (QGraphicsPolygonItem*)i->data(3).toULongLong();
                    delete header;
                }

                //Time to check what might have changed...

                if(newx != i->x() || newy != i->y()) {
                    i->setPos(newx, newy);
                }
                if(type == "rect") {
                    QGraphicsRectItem* r = (QGraphicsRectItem*)i;
                    if(QColor(d.value("fill_color").toString()) != r->brush().color()) {
                        r->setBrush(QColor(d.value("fill_color").toString()));
                    }
                } else if (type == "ellipse") {
                    QGraphicsEllipseItem* e = (QGraphicsEllipseItem*)i;
                    if(QColor(d.value("fill_color").toString()) != e->brush().color()) {
                        e->setBrush(QColor(d.value("fill_color").toString()));
                    }
                }
                found = true;
                break;
             }
        }

        if(!found) {
            if(type == "line") {
                QPen pen(QColor(d.value("color").toString()));
                pen.setWidth(2);
                QJsonObject start = d.value("start").toObject();
                QJsonObject end = d.value("end").toObject();
                QLineF line(start.value("x").toDouble(), start.value("y").toDouble(), end.value("x").toDouble(), end.value("y").toDouble());
                QGraphicsLineItem* l = addLine(line, pen);
                l->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                l->setCursor(Qt::PointingHandCursor);
                l->setData(0, d.value("sid").toInt());
                l->setData(1, "line");
                QJsonObject pos = d.value("scenepos").toObject();

            } else if (type == "rect") {
                QPen pen(QColor(d.value("outline_color").toString()));
                pen.setWidth(2);

                QBrush brush(QColor(d.value("fill_color").toString()));
                QJsonObject start = d.value("start").toObject();
                QJsonObject end = d.value("end").toObject();
                QRectF rect(start.value("x").toDouble(), start.value("y").toDouble(), end.value("x").toDouble(), end.value("y").toDouble());
                QGraphicsRectItem* r = addRect(rect, pen, brush);
                r->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                r->setCursor(Qt::PointingHandCursor);
                r->setData(0, d.value("sid").toInt());
                r->setData(1, "rect");
                QJsonObject pos = d.value("scenepos").toObject();

            } else if (type == "ellipse") {
                QPen pen(QColor(d.value("outline_color").toString()));
                pen.setWidth(2);

                QBrush brush(QColor(d.value("fill_color").toString()));
                QJsonObject start = d.value("start").toObject();
                QJsonObject end = d.value("end").toObject();
                QRectF rect(start.value("x").toDouble(), start.value("y").toDouble(), end.value("x").toDouble(), end.value("y").toDouble());
                QGraphicsEllipseItem* e = addEllipse(rect, pen, brush);
                e->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                e->setCursor(Qt::PointingHandCursor);
                e->setData(0, d.value("sid").toInt());
                e->setData(1, "ellipse");
                QJsonObject pos = d.value("scenepos").toObject();

            } else if (type == "text") {
                QString text = d.value("text").toString();
                QGraphicsTextItem* t = addText(text);
                QJsonObject start = d.value("start").toObject();
                t->setPos(start.value("x").toDouble(), start.value("y").toDouble());
                t->setDefaultTextColor(QColor(d.value("color").toString()));
                t->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                t->setCursor(Qt::PointingHandCursor);
                t->setData(0, d.value("sid").toInt());
                t->setData(1, "text");
                QJsonObject pos = d.value("scenepos").toObject();

            } else if (type == "latex") {
                std::cout << "CHAD HELP ME" << std::endl;
                string doc = "\\documentclass{standalone}\n\\begin{document}\n\\Huge $" + d.value("text").toString().toStdString() + "$\n\\end{document}";
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
                    QGraphicsPixmapItem* text = addPixmap(pix);
                    text->setPos(start.value("x").toDouble(), start.value("y").toDouble());
                    text->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    text->setFlag(QGraphicsItem::ItemIsMovable, true);
                    text->setCursor(Qt::PointingHandCursor);
                    text->setData(0, d.value("sid").toInt());
                    text->setData(1, "latex");
                    text->setData(2, d.value("text").toString());
                } else {
                    // otherwise, it was invalid latex
                    std::cout << "WAS NOT GOOD!"<< std::endl;
                    QGraphicsTextItem* text = addText(d.value("text").toString());
                    text->setDefaultTextColor(Qt::black);
                    text->setPos(start.value("x").toDouble(), start.value("y").toDouble());
                    text->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    text->setFlag(QGraphicsItem::ItemIsMovable, true);
                    text->setCursor(Qt::PointingHandCursor);
                    text->setData(0, d.value("sid").toInt());
                    text->setData(1, "text");
                }
                // text->setTextInteractionFlags(Qt::TextEditorInteraction);

            } else if (type == "arrow") {
                QPen pen(QColor(d.value("color").toString()));
                pen.setWidth(2);
                QJsonObject start = d.value("start").toObject();
                QJsonObject end = d.value("end").toObject();
                QLineF line(start.value("x").toDouble(), start.value("y").toDouble(), end.value("x").toDouble(), end.value("y").toDouble());
                QGraphicsLineItem* l = addLine(line, pen);
                l->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                l->setCursor(Qt::PointingHandCursor);
                l->setData(0, d.value("sid").toInt());
                l->setData(1, "arrow");
                QJsonObject pos = d.value("scenepos").toObject();

                qreal arrowSize = 20;
                double angle = std::atan2(-line.dy(), line.dx());
                QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                        cos(angle + M_PI / 3) * arrowSize);
                QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                        cos(angle + M_PI - M_PI / 3) * arrowSize);
                QPolygonF arrowHead;
                arrowHead << line.p1() << arrowP1 << arrowP2;
                QGraphicsPolygonItem* head = addPolygon(arrowHead,pen);
                head->setData(1, "arrowhead");

            }
        }
    }
}

int ProjectScene::trackItem(QGraphicsItem* item)
{

	QString type = item->data(1).toString();
	int id = m_tracked_items.size();
	item->setData(0, id);
    itemStats tracker = itemStats(m_board_id, item);
    m_tracked_items.push_back(tracker);
    cout << "TRACKER \n" << "QGraphicsItem pos: " << item->x() << " " << item->y() << endl;
    cout << "ItemStats pos: " << tracker.x << " " << tracker.y << endl;
    //item->setPos(tracker.x, tracker.y);
	return id;
	//Returns the id of the newly-tracked item.
}

void ProjectScene::checkPos()
{
	//QList<QGraphicsItem*> items = items();
	for(int j = 0; j <  m_tracked_items.size(); ++j) {
		itemStats x = m_tracked_items[j];
		for(QGraphicsItem* i : items()) {
			if (i->data(0).toInt() == x.id) { // Same item.
                //if(i->x() == 0 && i->y() == 0) continue;
				if(x.type == "line") {
					QGraphicsLineItem* l = (QGraphicsLineItem*)i;
					QLineF chk = l->line();
                    if(x.scenex == l->pos().x() && x.sceney == l->pos().y() && x.outline == l->pen().color()) continue;
                    std::cout << "Something changed about a line item" << std::endl;

					itemStats package(m_board_id, i);
					m_tracked_items[j] = package;
					QDataStream socketstream(m_socket);
                    socketstream.startTransaction();
					socketstream << package.byteData();
                    socketstream.commitTransaction();
				} else if (x.type == "text") {
					QGraphicsTextItem* t = (QGraphicsTextItem*)i;
					if(x.x == t->x() && x.y == t->y() && x.outline == t->defaultTextColor() && x.text == t->toPlainText().toStdString()) continue;
                    std::cout << "Something changed about a text item" << std::endl;

					itemStats package(m_board_id, i);
					m_tracked_items[j] = package;
					QDataStream socketstream(m_socket);
                    socketstream.startTransaction();
					socketstream << package.byteData();
                    socketstream.commitTransaction();
				} else {
					QGraphicsRectItem* r = (QGraphicsRectItem*)i;
                    QRectF chk = r->rect();
                    if(x.scenex == r->x() && x.sceney == r->y() && x.width == chk.width() && x.height == chk.height() && x.outline == r->pen().color() && x.fill == r->brush().color()) continue;

                    std::cout << "Something changed about a rect/ellipse item" << std::endl;
                    std::cout << "Tracker pos / graphics pos: " << x.x << " " << x.y << " " << r->x() << " " << r->y() << std::endl;
                    std::cout << "Scene pos: " << r->sceneBoundingRect().x() << " " << r->sceneBoundingRect().y() << std::endl;

					itemStats package(m_board_id, i);
					m_tracked_items[j] = package;

					QDataStream socketstream(m_socket);
                    std::cout << QJsonDocument(package.toJson()).toJson(QJsonDocument::Compact).toStdString() << std::endl;
                    socketstream.startTransaction();
					socketstream << package.byteData();
                    socketstream.commitTransaction();
				}
			}
		}
	}
}

//sends the data about the object that was on the scene to the server. 
void ProjectScene::sceneChanged(const QList<QRectF> &region) 
{

    // get a list of the items we need to update?
    QList<QGraphicsItem*> changed_items;
    for (QRectF r : region) {
      changed_items = items(r);
    }

    //Gets the items in the area that has changed/
    for (QGraphicsItem* i : changed_items) {

        if(i->data(0).toInt() == -1) {
		//Is this a fresh item?
        if(i->data(1).toString() == "arrowhead") {
            continue;
        }
		int new_id = trackItem(i);
		std::cout << "Fresh item! ID: " << new_id << std::endl;
		//Immediately tracks the item if it's fresh
		//Sets the ID as well
    }



         else { /*
            itemStats checker;
            for(itemStats j : m_tracked_items) {
                if(j.id == i->data(0).toInt()) {
                    checker = j;
                    break;
                } //Finds the item with matching ID from the internally tracked things.
            }

            //This if statement is going to be nasty. All it's going to do is check the item's current state with its last-known state. It does this by checking every variable in the itemStats class against what it presently is in the scene.
            //It should be accurate to find out which item is changed.
	    if(i->data(0).toString() != "line") {
		    QGraphicsRectItem* r = (QGraphicsRectItem*)i;
		    QRectF chk = r->rect();
		    if((chk.x() == checker.x) && (chk.y() == checker.y) && (chk.height() == checker.height) && (chk.width() == checker.width)) { 
			    //If nothing changed... 
			    //Currently does not check color
			    continue;
            	}
	    } else {
		    QGraphicsLineItem* l = (QGraphicsLineItem*)i;
		    QLineF chk = l->line();
		    if((chk.x1() == checker.x) && (chk.y1() == checker.y) && (chk.y2() == checker.height) && (chk.x2() == checker.width)) { 
			    continue;
	    }
		    //To do:
		    //The above functions also need to check if the color changed.
		    //The above if statements will also need to be updated as we add more variables to the item.
		    //Right now, for whatever reason, the above if statements do not trigger if the item is moved.
		    //Needs to be debugged.

        } */
        continue;
    }

	//So if it's made it to this point, something has changed about
	//the item. It's time to send it out to the server.

	QByteArray data = itemStats(m_board_id, i).byteData();
	//Turns the item into a json, then turns it into a byte array.
	//See 'itemStats.h' for more info on how it does it.

	QDataStream socketstream(m_socket);
    socketstream.startTransaction();
	socketstream << data;
    socketstream.commitTransaction();
    	}
}
