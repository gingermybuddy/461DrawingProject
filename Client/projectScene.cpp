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
#include <QFileDialog>
#include <QInputDialog>

using namespace std;

ProjectScene::ProjectScene() 
{
    m_starting_file = nullptr;
    id_increment = 0;
    //setSceneRect(0, 0, 800, 800);
}

bool ProjectScene::connectToBoard(QHostAddress ip, int port, std::string board_id)
{
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
        m_socket->connectToHost(ip, port);
        if(m_socket->waitForConnected()){
                std::cout << "Connected." << std::endl;
        } else {
        std::cout << "Failed to connect to server." << std::endl;
        return false;
    }
    m_board_id = board_id;
    QJsonObject boardcon;
    boardcon.insert("board_connection", QJsonValue(QString::fromStdString(board_id)));

    if(m_starting_file != nullptr) {
        QJsonDocument loader = *m_starting_file;
        boardcon.insert("load_file", QJsonValue(loader.object()));
    }
    QByteArray boardreq = QJsonDocument(boardcon).toJson();
    QDataStream sockstream(m_socket);

    sockstream << boardreq;
    return true;
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
        if(m_starting_file != nullptr) {
            emit file_already_loaded();
            return;
        }
        obj.remove("fullUpdate");

        int largest_id = 0;
        std::vector<QJsonObject> shapes;
        foreach(QString str, obj.keys()) {
            shapes.push_back(obj.value(str).toObject());
        }
        updateCanvas(shapes);
        for(QGraphicsItem* i : items()){
            if(i->data(0).toInt() > largest_id){
                largest_id = i->data(0).toInt();
            }
            m_tracked_items.push_back(itemStats(m_board_id, i));
            id_increment = largest_id;
        }
		return;
    }

    if(obj.value("delete_item") != QJsonValue::Undefined) {
        int del_id = obj.value("delete_item").toInt();
        for(auto it = m_tracked_items.begin(); it != m_tracked_items.end(); ++it) {
            if(it->id == del_id) {
                m_tracked_items.erase(it);
            }
        }
        for(QGraphicsItem* i : items()) {
            if(i->data(0).toInt() == del_id) {
                delete i;
            }
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

                qreal arrowSize = 20;
                double angle = std::atan2(-line.dy(), line.dx());
                QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                        cos(angle + M_PI / 3) * arrowSize);
                QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                        cos(angle + M_PI - M_PI / 3) * arrowSize);
                QPolygonF arrowHead;
                std::cout << "Placing an arrow from: " << line.p1().x() << " " << line.p1().y() << " to " << line.p2().x() << " " << line.p2().y() << std::endl;
                arrowHead << arrowP1 << arrowP2 << line.p1() << line.p2() << line.p1();
                QGraphicsPolygonItem* head = addPolygon(arrowHead,pen);
                head->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                head->setCursor(Qt::PointingHandCursor);
                head->setData(0, -1);
                head->setData(1, "arrow");

            } else if (type == "bezier") {
                QPen pen(QColor(d.value("color").toString()));
                pen.setWidth(2);
                QJsonObject start = d.value("start").toObject();
                QJsonObject end = d.value("end").toObject();
                qreal x = start.value("x").toDouble();
                qreal y = start.value("y").toDouble();
                qreal x2 = end.value("x").toDouble();
                qreal y2 = end.value("y").toDouble();

                QPainterPath path;
                // move path to start
                path.moveTo(x,y);
                // XXX calculate midpoint
                qreal midx = (x+x2)/2;
                qreal midy = (y+y2)/2;

                qreal slopex = x2-midx;
                qreal slopey = y2-midy;

                qreal cx = midx - 25*sin(atan(slopey/slopex));
                qreal cy = midy + 25*cos(atan(slopey/slopex));

                if (x2 < x){
                    cy = midy - 25*cos(atan(slopey/slopex));
                }

                // XXX do transformation
                path.quadTo(cx, cy ,x2,y2);

                QGraphicsPathItem* line = addPath(path, pen);
                line->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                line->setCursor(Qt::PointingHandCursor);
                line->setData(0, -1);
                line->setData(1, "bezier");

            }
        }
    }
}

int ProjectScene::trackItem(QGraphicsItem* item)
{

	QString type = item->data(1).toString();
    ++id_increment;
    item->setData(0, id_increment);
    itemStats tracker = itemStats(m_board_id, item);
    m_tracked_items.push_back(tracker);
    cout << "TRACKER \n" << "QGraphicsItem pos: " << item->x() << " " << item->y() << endl;
    cout << "ItemStats pos: " << tracker.x << " " << tracker.y << endl;
    //item->setPos(tracker.x, tracker.y);
    return id_increment;
	//Returns the id of the newly-tracked item.
}

void ProjectScene::deleteItem(int did)
{
    for(auto it = m_tracked_items.begin(); it != m_tracked_items.end(); ++it) {
        if(it->id == did) {
            m_tracked_items.erase(it);
            QJsonObject deldata;
            deldata.insert("delete", QJsonValue(did));
            deldata.insert("del_board_id", QJsonValue(QString::fromStdString(m_board_id)));

            QByteArray delreq = QJsonDocument(deldata).toJson();
            QDataStream socketstream(m_socket);
            socketstream.startTransaction();
            socketstream << deldata;
            socketstream.commitTransaction();

        }
    }
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
                    if(x.scenex == l->pos().x() && x.sceney == l->pos().y() && x.outline == l->pen().color()) continue;
                    std::cout << "Something changed about a line item" << std::endl;

					itemStats package(m_board_id, i);
					m_tracked_items[j] = package;

                    sendItem(package);

				} else if (x.type == "text") {
					QGraphicsTextItem* t = (QGraphicsTextItem*)i;
					if(x.x == t->x() && x.y == t->y() && x.outline == t->defaultTextColor() && x.text == t->toPlainText().toStdString()) continue;
                    std::cout << "Something changed about a text item" << std::endl;

					itemStats package(m_board_id, i);
					m_tracked_items[j] = package;

                    sendItem(package);

                } else if (x.type == "latex") {
                    QGraphicsPixmapItem* t = (QGraphicsPixmapItem*)i;
                    if(x.x == t->x() && x.y == t->y() && x.text == t->data(2).toString().toStdString()) continue;
                    std::cout << "Something changed about a latex item" << std::endl;
                    itemStats package(m_board_id, i);
                    m_tracked_items[j] = package;

                    sendItem(package);

                } else if (x.type == "rect" || x.type == "ellipse") {
					QGraphicsRectItem* r = (QGraphicsRectItem*)i;
                    QRectF chk = r->rect();
                    if(x.scenex == r->x() && x.sceney == r->y() && x.width == chk.width() && x.height == chk.height() && x.outline == r->pen().color() && x.fill == r->brush().color()) continue;

                    std::cout << "Something changed about a rect/ellipse item" << std::endl;
                    std::cout << "Tracker pos / graphics pos: " << x.x << " " << x.y << " " << r->x() << " " << r->y() << std::endl;
                    std::cout << "Scene pos: " << r->sceneBoundingRect().x() << " " << r->sceneBoundingRect().y() << std::endl;

					itemStats package(m_board_id, i);
					m_tracked_items[j] = package;

                    sendItem(package);
                } else if (x.type == "arrow") {

                    QGraphicsPolygonItem* a = (QGraphicsPolygonItem*)i;
                    if(x.scenex == a->pos().x() && x.sceney == a->pos().y() && x.outline == a->pen().color()) continue;
                    std::cout << "Something changed about an arrow item" << std::endl;

                    itemStats package(m_board_id, i);
                    m_tracked_items[j] = package;
                    sendItem(package);
                } else if (x.type == "bezier") {
                    QGraphicsPathItem* p = (QGraphicsPathItem*)i;
                    if(x.scenex == p->pos().x() && x.sceney == p->pos().y() && x.outline == p->pen().color()) continue;
                    std::cout << "Something changed about a bezier curve" << std::endl;

                    itemStats package(m_board_id, i);
                    m_tracked_items[j] = package;
                    sendItem(package);
                }
			}
		}
	}
}

void ProjectScene::sendItem(itemStats package) //Convenience function to avoid duplicating too much code
{
    QDataStream socketstream(m_socket);
    std::cout << QJsonDocument(package.toJson()).toJson(QJsonDocument::Compact).toStdString() << std::endl;
    //above cout statement for debugging purposes
    socketstream.startTransaction();
    socketstream << package.byteData();
    socketstream.commitTransaction();
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
		int new_id = trackItem(i);
		std::cout << "Fresh item! ID: " << new_id << std::endl;
		//Immediately tracks the item if it's fresh
		//Sets the ID as well
    }
         else {continue;}

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

QJsonObject ProjectScene::exportToFile()
{
    QJsonObject returnval;
    for(QGraphicsItem* i : items()) {
        itemStats item = itemStats(m_board_id, i);
        returnval.insert(QString::number(item.id), item.toJson());
    }
    return returnval;
}

void ProjectScene::loadFile(QJsonDocument doc)
{
    m_starting_file = new QJsonDocument(doc);
    QJsonObject obj = doc.object();
    std::vector<QJsonObject> shapes;
    foreach(QString str, obj.keys()) {
        shapes.push_back(obj.value(str).toObject());
    }
    updateCanvas(shapes);
    for(QGraphicsItem* i : items()){
        m_tracked_items.push_back(itemStats(m_board_id, i));
    }
}
