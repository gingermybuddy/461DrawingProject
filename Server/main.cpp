#include <QApplication>
#include "Server.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc < 2) {
        std::cout << "Please specify a port." << std::endl;
        return -1;
    }
	QApplication new_window(argc, argv);
    Server w(std::stoi(argv[1]));
    return new_window.exec();
}
