#include "window.h"
#include <QMessageBox>
#include <QLayout>
#include <iostream>


Window::Window() : QMainWindow()
{
	m_toolbar = new QToolBar(this); 
	//Anything new you add to the window is going to be a pointer to whatever object you're throwing in.
	//This sets up a toolbar inside the mainwindow with the 'this' call.
	
	addToolBar(m_toolbar); //Adds it to the display
	m_circle_button = new QAction(tr("Circle"));
	m_line_button = new QAction(tr("Line"));
	//This sets up an action that you can place on the toolbar.
	//The tr() function translates a regular C++ string into Qt's internal string (QString).

	m_toolbar->addSeparator();
	m_toolbar->setMovable(false);
	// m_toolbar->addAction(m_testoption);
	m_toolbar->addAction(m_circle_button);
	m_toolbar->addAction(m_line_button);
	//Separators are just decorative.
	
	m_scene = new ProjectScene();
	m_view = new ProjectView; //Custom graphics scene where we can overload stuff
    m_view->change_tool(1); // set default tool to line
	m_view->setScene(m_scene);
	
	// connect(m_testoption, SIGNAL(triggered()), this, SLOT(popup()));
	connect(m_circle_button, SIGNAL(triggered()), this, SLOT(set_circle()));
	connect(m_line_button, SIGNAL(triggered()), this, SLOT(set_line()));
	connect(m_scene, SIGNAL(changed(const QList<QRectF>&)), m_scene, SLOT(sceneChanged(const QList<QRectF>&)));
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
}

void Window::popup()
{
	QMessageBox popup;
	popup.setWindowTitle(tr("Popup!"));
	popup.setText("I'm distracting!");
	popup.exec();
	//Pretty much what it says on the tin.
}

void Window::set_line()
{
    m_view->change_tool(1);
    std::cout << "Changing tool" << std::endl;
}

void Window::set_circle()
{
    m_view->change_tool(2);
}

void Window::place_rectangle()
{
}


