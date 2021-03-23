#include <QMainWindow>
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include "projectView.h"
#include "projectScene.h"
#include "toolBar.h"
#ifndef __WINDOW_H
#define __WINDOW_H

class Window : public QMainWindow //Extension on the base QMainWindow class
{
	Q_OBJECT //Must be included for qmake to recognize this

	private: //Menus and features of the window
		ToolBar* m_bar;
		QDockWidget* m_tool_dock;
		ProjectScene* m_scene;
		ProjectView* m_view;

        // shape tools

	private slots: //Where functions attached to buttons go
        // make a popup window
		void popup();
	public:
		Window();
		~Window();
};

#endif
