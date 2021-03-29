#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "projectView.h"
#include <QColorDialog>

#ifndef __TOOLBAR_H
#define __TOOLBAR_H

class ToolBar : public QWidget
{
	Q_OBJECT
	private:
		QPushButton* m_circle;
		QPushButton* m_line;
		QPushButton* m_rect;
		QPushButton* m_default;
		QPushButton* m_black;
		QPushButton* m_red;
		QPushButton* m_green;
		QPushButton* m_yellow;
		QPushButton* m_blue;
		QPushButton* m_color_picker;
		QVBoxLayout* m_layout;
		QPushButton* m_fill;
		ProjectView* m_view;
	public slots:
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
            void fill();
	public:
		ToolBar();
		~ToolBar();
		void set_view(ProjectView* view);
	
};
#endif
