#include <QApplication>
#include "window.h"
#include <iostream>

int main(int argc, char* argv[])
{
	QApplication new_window(argc, argv);
	Window w;
	w.show();
	w.resize(500,500);
	return new_window.exec();
}
