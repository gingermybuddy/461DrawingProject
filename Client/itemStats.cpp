#include "itemStats.h"
#include <QPen>

itemStats::itemStats()
{
}

//UPDATE THIS IF YOU ADD THINGS TO THE PLACED ITEMS
itemStats::itemStats(std::string nboard_id, std::string ntype, int nid, double nx, double ny, double nheight, double nwidth,  double nscenex, double nsceney, QColor nfill, QColor noutline)
{
	board_id = nboard_id;
	type = ntype;
	id = nid;
	x = nx;
	y = ny;
	height = nheight;
	width = nwidth;
    scenex = nscenex;
    sceney = nsceney;
	fill = nfill;
	outline = noutline;
}

itemStats::itemStats(std::string nboard_id, std::string ntype,int nid, double nx, double ny, std::string ntext, QColor nrgb)
{
	board_id = nboard_id;
	type = ntype;
	id = nid;
	x = nx;
	y = ny;
	text = ntext;
	outline = nrgb;
}


itemStats::itemStats(std::string nboard_id, std::string ntype,int nid, double nx, double ny, double nheight, double nwidth,  double nscenex, double nsceney, QColor nrgb)
{
	board_id = nboard_id;
	type = ntype;
	id = nid;
	x = nx;
	y = ny;
	height = nheight;
	width = nwidth;
    scenex = nscenex;
    sceney = nsceney;
	outline = nrgb;
}

//UPDATE THIS IF YOU ADD THINGS TO THE PLACED ITEMS
itemStats::itemStats(std::string nboard_id, QGraphicsItem* item)
{
	//Parses the QGraphicsItem and throws the variables
	//into their appropriate places.

	board_id = nboard_id;
	type = item->data(1).toString().toStdString();
	id = item->data(0).toInt();
    scenex = item->x();
    sceney = item->y();

	if(type == "line") {
		QGraphicsLineItem* i = (QGraphicsLineItem*)item;
		x = i->line().x1();
		y = i->line().y1();
		height = i->line().y2();
		width = i->line().x2();
		outline = i->pen().color();
	} else if(type == "text") { 
		QGraphicsTextItem* t = (QGraphicsTextItem*)item;
		x = t->x();
		y = t->y();
		outline = t->defaultTextColor();
		text = t->toPlainText().toStdString();
    } else if (type == "arrow") {
        QGraphicsLineItem* i = (QGraphicsLineItem*) item;
        x = i->line().x1();
        y = i->line().y1();
        height = i->line().y2();
        width = i->line().x2();
        outline = i->pen().color();
    } else if (type == "latex") {
		QGraphicsPixmapItem* t = (QGraphicsPixmapItem*)item;
		x = t->x();
		y = t->y();
		text = t->data(2).toString().toStdString();

    } else {
		QGraphicsRectItem* r = (QGraphicsRectItem*)item;
        x = r->rect().x();
        y = r->rect().y();
		height = r->rect().height();
		width = r->rect().width();
        outline = r->pen().color();
        fill = r->brush().color();
	}
}

itemStats::~itemStats()
{
}

//UPDATE THIS IF YOU ADD THINGS TO THE PLACED ITEMS
QJsonObject itemStats::toJson()
{
	//Turns the item into a JSON object, formatted as it would
	//be in the jsonExamples folder.
	//This doesn't actually differ at all from different shape
	//types.

	QJsonObject returnval;
	QJsonObject data;
	QJsonObject start;
	QJsonObject end;
    QJsonObject scenepos;
	data.insert("bid", QJsonValue(QString::fromStdString(board_id)));

	start.insert("x", QJsonValue(x));
	start.insert("y", QJsonValue(y));

    scenepos.insert("scenex", QJsonValue(scenex));
    scenepos.insert("sceney", QJsonValue(sceney));
    data.insert("scenepos", QJsonValue(scenepos));

	if(type == "text") {
		data.insert("color", QJsonValue(outline.name()));
		data.insert("start", QJsonValue(start));
        data.insert("sid", QJsonValue(id)),
		data.insert("text", QJsonValue(QString::fromStdString(text)));
		returnval.insert("data", QJsonValue(data));
		returnval.insert("shape", QJsonValue(QString::fromStdString(type)));

    } else if (type =="latex") {
        data.insert("color", QJsonValue("#000000"));
        data.insert("start", QJsonValue(start));
        data.insert("sid", QJsonValue(id));
        data.insert("text", QJsonValue(QString::fromStdString(text)));
        returnval.insert("data", QJsonValue(data));
        returnval.insert("shape", QJsonValue(QString::fromStdString(type)));

    } else if (type == "line" || type == "arrow") {
		end.insert("x", QJsonValue(width));
		end.insert("y", QJsonValue(height));
		data.insert("color", QJsonValue(outline.name()));
		data.insert("end", QJsonValue(end));
		data.insert("sid", QJsonValue(id));
		data.insert("start", QJsonValue(start));
		returnval.insert("data", QJsonValue(data));
		returnval.insert("shape", QJsonValue(QString::fromStdString(type)));
	} else {
		end.insert("x", QJsonValue(width));
		end.insert("y", QJsonValue(height));
        data.insert("fill_color", QJsonValue(fill.name(QColor::HexArgb)));
		data.insert("outline_color", QJsonValue(outline.name()));
		data.insert("end", QJsonValue(end));
		data.insert("sid", QJsonValue(id));
		data.insert("start", QJsonValue(start));
		returnval.insert("data", QJsonValue(data));
		returnval.insert("shape", QJsonValue(QString::fromStdString(type)));
	}

	//For the 'end' parameter, it uses the width and height as x and y coordinates.
	//If the thing's a line, the end is just what it says on the tin - the ending coordinates of the line.
	//
	return returnval;
}

QByteArray itemStats::byteData()
{
	//Turns it first into a JSON object, then passes that object
	//into a QByteArray. This array can be sent straight
	//to the server.
	QJsonDocument doc(toJson());
	QByteArray data = doc.toJson();
	return data;
}
