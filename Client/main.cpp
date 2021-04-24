#include <QApplication>
#include "window.h"
#include <iostream>

int main(int argc, char* argv[])
{
	QApplication new_window(argc, argv);
	Window w;
	w.resize(1200,800);
	return new_window.exec();
}
