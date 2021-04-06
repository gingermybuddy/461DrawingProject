#include "window.h"
#include "qcolordialog.h"
#include <QMessageBox>
#include <QLayout>
#include <QColorDialog>
#include <iostream>

Window::Window() : QMainWindow()
{
	//Anything new you add to the window is going to be a pointer to whatever object you're throwing in.
	//This sets up a toolbar inside the mainwindow with the 'this' call.
	
	m_scene = new ProjectScene();
    m_scene->setBackgroundBrush(Qt::white);
	m_view = new ProjectView; //Custom graphics scene where we can overload stuff
	m_view->change_tool(1); // set default tool to line
	m_view->change_color(255,0,0); // set default color to black
	m_view->setScene(m_scene);

	m_bar = new ToolBar();
	m_bar->set_view(m_view);
	m_tool_dock = new QDockWidget();
	m_tool_dock->setWidget(m_bar);

	m_tool_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	// connect(m_testoption, SIGNAL(triggered()), this, SLOT(popup()));
	connect(m_scene, SIGNAL(changed(const QList<QRectF>&)), m_scene, SLOT(sceneChanged(const QList<QRectF>&)));

	//connect(), as shown, will connect a given action (or button) with a function in private slots. This just sets it so when you actually click on it, something happens.
	setCentralWidget(m_view);
	addDockWidget(Qt::LeftDockWidgetArea, m_tool_dock);
	this->show();

}

Window::~Window()
{
	// delete m_testoption;
}

void Window::popup()
{
	QMessageBox popup;
	popup.setWindowTitle(tr("Popup!"));
	popup.setText("I'm distracting!");
	popup.exec();
	//Pretty much what it says on the tin.
}

