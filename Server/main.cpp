#include <QApplication>
#include "Server.h"
#include <iostream>

int main(int argc, char* argv[])
{
	QApplication new_window(argc, argv);
    Server w(std::stoi(argv[1]));
    return new_window.exec();
}
