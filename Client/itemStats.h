#include <QColor>
#include <QJsonObject>
#include <QByteArray>
#include <QString>
#include <QJsonDocument>

#ifndef __ITEMSTATS_H
#define __ITEMSTATS_H
class itemStats
{
	public:
		std::string board_id;
        	std::string type;
        	int id;
        	double x;
        	double y;
        	double height;
        	double width;
        	QColor rgb;

		QJsonObject toJson();
		QByteArray byteData();
		itemStats();
		itemStats(std::string board_id, std::string type, int id, double x, double y, double height, double width, QColor rgb);
		~itemStats();
};

#endif

