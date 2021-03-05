#include <QMainWindow>
#include <QWidget>
#include <QToolBar>
#include <QAction>

#ifndef __WINDOW_H
#define __WINDOW_H

class Window : public QMainWindow //Extension on the base QMainWindow class
{
	Q_OBJECT //Must be included for qmake to recognize this

	private: //Menus and features of the window
		QToolBar* m_toolbar;
		QAction* m_testoption;
	private slots: //Where functions attached to buttons go
		void popup();
	public:
		Window();
		~Window();
};

#endif
