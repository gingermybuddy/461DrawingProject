#include "window.h"
#include "qcolordialog.h"
#include <QMessageBox>
#include <QLayout>
#include <QColorDialog>
#include <iostream>


Window::Window() : QMainWindow()
{
	m_toolbar = new QToolBar(this); 
	m_colorbar = new QToolBar(this); 
	//Anything new you add to the window is going to be a pointer to whatever object you're throwing in.
	//This sets up a toolbar inside the mainwindow with the 'this' call.
	
	addToolBar(m_toolbar); // Adds it to the display
	addToolBar(m_colorbar); // add the colorbar to the display

	m_circle_button = new QAction(QIcon("./icons/tool_circle.png"), tr("Circle"));
	m_line_button = new QAction(QIcon("./icons/tool_line.png"), tr("Line"));
    m_rect_button = new QAction(QIcon("./icons/tool_rect.png"), tr("Rectangle"));
	m_default_button = new QAction(QIcon("./icons/tool_select.png"), tr("Select"));
	//This sets up an action that you can place on the toolbar.
	//The tr() function translates a regular C++ string into Qt's internal string (QString).

    // add color buttons
	m_color_black = new QAction(QIcon("./icons/black.png"), tr("Black"));
	m_color_red = new QAction(QIcon("./icons/red.png"), tr("Red"));
	m_color_green = new QAction(QIcon("./icons/green.png"), tr("Green"));
	m_color_yellow = new QAction(QIcon("./icons/yellow.png"), tr("Yellow"));
	m_color_blue = new QAction(QIcon("./icons/blue.png"), tr("Blue"));
	m_color_picker = new QAction(QIcon("./icons/colorPicker.png"), tr("Color Picker"));

	m_toolbar->addSeparator();
	m_toolbar->setMovable(false);
	m_colorbar->setMovable(false);
	// m_toolbar->addAction(m_testoption);
	m_toolbar->addAction(m_default_button);
	m_toolbar->addAction(m_circle_button);
	m_toolbar->addAction(m_line_button);
	m_toolbar->addAction(m_rect_button);
	//Separators are just decorative.

    m_colorbar->addAction(m_color_black);
    m_colorbar->addAction(m_color_red);
    m_colorbar->addAction(m_color_green);
    m_colorbar->addAction(m_color_yellow);
    m_colorbar->addAction(m_color_blue);
    m_colorbar->addAction(m_color_picker);
	
	m_scene = new ProjectScene();
    m_scene->setBackgroundBrush(Qt::white);
	m_view = new ProjectView; //Custom graphics scene where we can overload stuff
	m_view->change_tool(1); // set default tool to line
	m_view->change_color(255,0,0); // set default color to black
	m_view->setScene(m_scene);
	
	// connect(m_testoption, SIGNAL(triggered()), this, SLOT(popup()));
    // bind shape buttons
	connect(m_default_button, SIGNAL(triggered()), this, SLOT(set_default()));
	connect(m_circle_button, SIGNAL(triggered()), this, SLOT(set_circle()));
	connect(m_line_button, SIGNAL(triggered()), this, SLOT(set_line()));
    connect(m_rect_button, SIGNAL(triggered()), this, SLOT(place_rectangle()));
	connect(m_scene, SIGNAL(changed(const QList<QRectF>&)), m_scene, SLOT(sceneChanged(const QList<QRectF>&)));

    // bind color buttons
    connect(m_color_black, SIGNAL(triggered()), this, SLOT(set_color_black()));
    connect(m_color_red, SIGNAL(triggered()), this, SLOT(set_color_red()));
    connect(m_color_green, SIGNAL(triggered()), this, SLOT(set_color_green()));
    connect(m_color_yellow, SIGNAL(triggered()), this, SLOT(set_color_yellow()));
    connect(m_color_blue, SIGNAL(triggered()), this, SLOT(set_color_blue()));
    connect(m_color_picker, SIGNAL(triggered()), this, SLOT(set_color_custom()));
	//connect(), as shown, will connect a given action (or button) with a function in private slots. This just sets it so when you actually click on it, something happens.
	
	this->layout()->setMenuBar(m_toolbar);
	setCentralWidget(m_view);
	this->show();

}

Window::~Window()
{
	delete m_toolbar;
	// delete m_testoption;
	delete m_circle_button;
	delete m_line_button;
	delete m_rect_button;
}

void Window::popup()
{
	QMessageBox popup;
	popup.setWindowTitle(tr("Popup!"));
	popup.setText("I'm distracting!");
	popup.exec();
	//Pretty much what it says on the tin.
}

void Window::set_default()
{
	m_view->change_tool(0);
}

void Window::set_line()
{
	m_view->change_tool(1);
    // std::cout << "Changing tool" << std::endl;
}

void Window::set_circle()
{
	m_view->change_tool(2);
}

void Window::set_color_black()
{
    m_view->change_color(0,0,0);
}
void Window::set_color_red()
{
    m_view->change_color(255,0,0);
}
void Window::set_color_green()
{
    m_view->change_color(0,255,0);
}
void Window::set_color_yellow()
{
    m_view->change_color(255,255,0);
}
void Window::set_color_blue()
{
    m_view->change_color(0,0,255);
}
void Window::set_color_custom()
{
    QColor custom = QColorDialog(this).getColor(Qt::white);
    m_view->change_color(custom.red(), custom.green(), custom.blue());
    
}

void Window::place_rectangle()
{
	m_view->change_tool(3);
}


