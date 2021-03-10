#include <QMainWindow>
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include "ProjectView.h"

#ifndef __WINDOW_H
#define __WINDOW_H

class Window : public QMainWindow //Extension on the base QMainWindow class
{
	Q_OBJECT //Must be included for qmake to recognize this

	private: //Menus and features of the window
		QGraphicsScene* m_scene;
		ProjectView* m_view;
		QToolBar* m_toolbar;
		QAction* m_testoption;
		QAction* m_rect_placer;
	private slots: //Where functions attached to buttons go
		void popup();
		void place_rectangle();
	public:
		Window();
		~Window();
};

#endif
