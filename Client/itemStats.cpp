#include "itemStats.h"
#include <QPen>

itemStats::itemStats()
{
}

//UPDATE THIS IF YOU ADD THINGS TO THE PLACED ITEMS
itemStats::itemStats(std::string nboard_id, std::string ntype, int nid, double nx, double ny, double nheight, double nwidth, QColor nrgb)
{
	board_id = nboard_id;
	type = ntype;
	id = nid;
	x = nx;
	y = ny;
	height = nheight;
	width = nwidth;
	rgb = nrgb;
}

//UPDATE THIS IF YOU ADD THINGS TO THE PLACED ITEMS
itemStats::itemStats(std::string nboard_id, QGraphicsItem* item)
{
	//Parses the QGraphicsItem and throws the variables
	//into their appropriate places.

	board_id = nboard_id;
	type = item->data(1).toString().toStdString();
	id = item->data(0).toInt();
	if(type == "line") {
		QGraphicsLineItem* i = (QGraphicsLineItem*)item;
		x = i->line().x1();
		y = i->line().y1();
		height = i->line().y2();
		width = i->line().x2();
		rgb = i->pen().color();
	} else {
		QGraphicsRectItem* r = (QGraphicsRectItem*)item;
		x = r->rect().x();
		y = r->rect().y();
		height = r->rect().height();
		width = r->rect().width();
		rgb = r->pen().color();
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

	data.insert("bid", QJsonValue(QString::fromStdString(board_id)));
	data.insert("color", QJsonValue(rgb.name()));

	start.insert("x", QJsonValue(x));
	start.insert("y", QJsonValue(y));

	end.insert("x", QJsonValue(width));
	end.insert("y", QJsonValue(height));

	data.insert("end", QJsonValue(end));
	data.insert("sid", QJsonValue(id));
	data.insert("start", QJsonValue(start));

	returnval.insert("data", QJsonValue(data));
	returnval.insert("shape", QJsonValue(QString::fromStdString(type)));

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
