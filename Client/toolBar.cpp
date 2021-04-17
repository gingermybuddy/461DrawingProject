#include "toolBar.h"

ToolBar::ToolBar() : QWidget()
{

	m_circle = new QPushButton(QIcon("./icons/tool_circle.png"), tr("Circle"));
	m_line = new QPushButton(QIcon("./icons/tool_line.png"), tr("Line"));
	m_rect = new QPushButton(QIcon("./icons/tool_rect.png"), tr("Rectangle"));
	m_default = new QPushButton(QIcon("./icons/tool_select.png"), tr("Select"));
	m_black = new QPushButton(QIcon("./icons/black.png"), tr("Black"));
	m_red = new QPushButton(QIcon("./icons/red.png"), tr("Red"));
	m_green = new QPushButton(QIcon("./icons/green.png"), tr("Green"));
	m_yellow = new QPushButton(QIcon("./icons/yellow.png"), tr("Yellow"));
	m_blue = new QPushButton(QIcon("./icons/blue.png"), tr("Blue"));
	m_color_picker = new QPushButton(QIcon("./icons/colorPicker.png"), tr("Color Picker"));
    m_fill = new QPushButton(QIcon("./icons/fill.png"), tr("Fill"));
	m_text = new QPushButton(QIcon("./icons/text.png"), tr("Text"));
	m_latex = new QPushButton(QIcon("./icons/text.png"), tr("Math"));
    m_arrow = new QPushButton(QIcon("./icons/arrow.png"), tr("Arrow"));
    m_bezier = new QPushButton(QIcon("./icons/line.png"), tr("Bezier"));
	m_layout = new QVBoxLayout(this);

	m_layout->addWidget(m_circle);
	m_layout->addWidget(m_line);
	m_layout->addWidget(m_rect);
	m_layout->addWidget(m_default);
	m_layout->addWidget(m_black);
	m_layout->addWidget(m_red);
	m_layout->addWidget(m_green);
	m_layout->addWidget(m_yellow);
	m_layout->addWidget(m_blue);
	m_layout->addWidget(m_color_picker);
   	m_layout->addWidget(m_fill);
	m_layout->addWidget(m_text);
	m_layout->addWidget(m_latex);
	m_layout->addWidget(m_arrow);
	m_layout->addWidget(m_bezier);

	connect(m_default, SIGNAL(clicked()), this, SLOT(set_default()));
    	connect(m_circle, SIGNAL(clicked()), this, SLOT(set_circle()));
   	connect(m_line, SIGNAL(clicked()), this, SLOT(set_line()));
    	connect(m_rect, SIGNAL(clicked()), this, SLOT(place_rectangle()));
	connect(m_text, SIGNAL(clicked()), this, SLOT(set_text()));
	connect(m_latex, SIGNAL(clicked()), this, SLOT(set_latex()));
	connect(m_arrow, SIGNAL(clicked()), this, SLOT(set_arrow()));
	connect(m_bezier, SIGNAL(clicked()), this, SLOT(set_bezier()));

    // bind color buttons
    connect(m_black, SIGNAL(clicked()), this, SLOT(set_color_black()));
    connect(m_red, SIGNAL(clicked()), this, SLOT(set_color_red()));
    connect(m_green, SIGNAL(clicked()), this, SLOT(set_color_green()));
    connect(m_yellow, SIGNAL(clicked()), this, SLOT(set_color_yellow()));
    connect(m_blue, SIGNAL(clicked()), this, SLOT(set_color_blue()));
    connect(m_color_picker, SIGNAL(clicked()), this, SLOT(set_color_custom()));
    connect(m_fill, SIGNAL(clicked()), this, SLOT(fill()));
 	
	setLayout(m_layout);

}

ToolBar::~ToolBar()
{
	delete m_circle;
	delete m_line;
	delete m_rect;
	delete m_default;
	delete m_black;
	delete m_red;
	delete m_green;
	delete m_yellow;
	delete m_blue;
	delete m_color_picker;
    	delete m_fill;
	delete m_text;
	delete m_arrow;
	delete m_bezier;
	delete m_layout;
}

void ToolBar::set_view(ProjectView* view)
{
	m_view = view;
}

void ToolBar::set_default()
{
        m_view->change_tool(0);
}

void ToolBar::set_line()
{
        m_view->change_tool(1);
    // std::cout << "Changing tool" << std::endl;
}

void ToolBar::set_circle()
{
        m_view->change_tool(2);
}

void ToolBar::set_color_black()
{
    m_view->change_color(0,0,0);
}
void ToolBar::set_color_red()
{
    m_view->change_color(255,0,0);
}
void ToolBar::set_color_green()
{
    m_view->change_color(0,255,0);
}
void ToolBar::set_color_yellow()
{
    m_view->change_color(255,255,0);
}
void ToolBar::set_color_blue()
{
    m_view->change_color(0,0,255);
}
void ToolBar::set_color_custom()
{
    QColor custom = QColorDialog(this).getColor(Qt::white);
    m_view->change_color(custom.red(), custom.green(), custom.blue());

}
void ToolBar::place_rectangle()
{
        m_view->change_tool(3);
}
void ToolBar::fill()
{
	m_view->change_tool(4);
}
void ToolBar::set_text()
{
	m_view->change_tool(5);
}
void ToolBar::set_latex()
{
	m_view->change_tool(6);
}
void ToolBar::set_arrow()
{
	m_view->change_tool(7);
}
void ToolBar::set_bezier()
{
	m_view->change_tool(8);
}
