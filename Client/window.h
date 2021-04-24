#include <QMainWindow>
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include "projectView.h"
#include "projectScene.h"
#include "toolBar.h"
#include "StartWindow.h"
#include <QTcpSocket>

#ifndef __WINDOW_H
#define __WINDOW_H

class Window : public QMainWindow //Extension on the base QMainWindow class
{
	Q_OBJECT //Must be included for qmake to recognize this

	private: //Menus and features of the window
		ToolBar* m_bar;
        QToolBar* m_menu_bar;
        QAction* m_save_img;
        QAction* m_save_canvas;
		QDockWidget* m_tool_dock;
		ProjectScene* m_scene;
		ProjectView* m_view;
        StartWindow* m_start_menu;

        // shape tools

	private slots: //Where functions attached to buttons go
        // make a popup window
		void popup();
        void join_canvas();
        void create_canvas();
        void load_canvas();
        void saveToImage();
        void saveCanvas();
	public:
        ToolBar* get_m_bar();
        QDockWidget* get_m_tool_dock();
        ProjectScene* get_m_scene();
        ProjectView* get_m_view();
        void setup_window();
        void show_start();

		Window();
		~Window();
};

#endif
