#include <QApplication>
#include "Server.h"
#include <iostream>

int main(int argc, char* argv[])
{
	QApplication new_window(argc, argv);
	Server w;
	return new_window.exec();
}
