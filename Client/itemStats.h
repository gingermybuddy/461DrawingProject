#include <QColor>
#include <QJsonObject>
#include <QByteArray>
#include <QString>
#include <QJsonDocument>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>

#ifndef __ITEMSTATS_H
#define __ITEMSTATS_H
struct itemStats
{
	//Lines get tracked the same as everything else.
	//Set the width/height variables to x2 and y2, respectively.
	std::string board_id;
        std::string type;
        int id;
        double x;
        double y;
        double height;
        double width;
        QColor rgb;

	//Functions to turn the thing into a QJsonObject
	//and a QByteArray that can be sent directly over a socket.
	QJsonObject toJson();
	QByteArray byteData();

	//Several different constructors for a couple of different
	//uses. You can pass in nothing, a QGraphicsItem* for the
	//client end, or a bunch of variables that it will simply
	//insert.
	itemStats();
	itemStats(std::string board_id, QGraphicsItem* item);
	itemStats(std::string board_id, std::string type, int id, double x, double y, double height, double width, QColor rgb);
	~itemStats();
};

#endif

