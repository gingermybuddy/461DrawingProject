#include <QApplication>
#include "Server.h"
#include <iostream>

int main(int argc, char* argv[])
{
	QApplication new_window(argc, argv);
	Server w;
	w.show();
	w.resize(300,300);
	return new_window.exec();
}
