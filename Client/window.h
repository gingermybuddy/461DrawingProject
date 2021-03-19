#include <QMainWindow>
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include "projectView.h"
#include "projectScene.h"

#ifndef __WINDOW_H
#define __WINDOW_H

class Window : public QMainWindow //Extension on the base QMainWindow class
{
	Q_OBJECT //Must be included for qmake to recognize this

	private: //Menus and features of the window
		ProjectScene* m_scene;
		ProjectView* m_view;
		QToolBar* m_toolbar;
		QToolBar* m_colorbar;

        // shape tools
        QAction* m_circle_button;
        QAction* m_line_button;
		QAction* m_rect_button;

        // movement tools (default is select)
		QAction* m_default_button;

        // color buttons
        QAction* m_color_black;
        QAction* m_color_red;
        QAction* m_color_green;
        QAction* m_color_yellow;
        QAction* m_color_blue;
        QAction* m_color_picker;

	private slots: //Where functions attached to buttons go
        // make a popup window
		void popup();
        // set default
        void set_default();
        // shapes
        void place_rectangle();
        void set_line();
        void set_circle();
        // colors
        void set_color_black();
        void set_color_red();
        void set_color_green();
        void set_color_yellow();
        void set_color_blue();
        void set_color_custom();
	public:
		Window();
		~Window();
};

#endif
