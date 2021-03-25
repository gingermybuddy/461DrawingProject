#include "itemStats.h"

itemStats::itemStats()
{
}

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

itemStats::~itemStats()
{
}

QJsonObject itemStats::toJson()
{
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
	QJsonDocument doc(toJson());
	QByteArray data = doc.toJson();
	return data;
}
