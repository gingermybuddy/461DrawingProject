#include "window.h"
#include <QMessageBox>
#include <QLayout>

Window::Window() : QMainWindow()
{
	m_toolbar = new QToolBar(this); 
	//Anything new you add to the window is going to be a pointer to whatever object you're throwing in.
	//This sets up a toolbar inside the mainwindow with the 'this' call.
	
	addToolBar(m_toolbar); //Adds it to the display
	m_testoption = new QAction(tr("Click Me"));
	//This sets up an action that you can place on the toolbar.
	//The tr() function translates a regular C++ string into Qt's internal string (QString).

	m_toolbar->addSeparator();
	m_toolbar->setMovable(false);
	m_toolbar->addAction(m_testoption);
	//Separators are just decorative.
	
	m_scene = new ProjectScene();
	m_view = new ProjectView; //Custom graphics scene where we can overload stuff
	m_view->setScene(m_scene);
	
	connect(m_testoption, SIGNAL(triggered()), this, SLOT(popup()));
	connect(m_scene, SIGNAL(changed(const QList<QRectF>&)), m_scene, SLOT(sceneChanged(const QList<QRectF>&)));
	//connect(), as shown, will connect a given action (or button) with a function in private slots. This just sets it so when you actually click on it, something happens.
	
	this->layout()->setMenuBar(m_toolbar);
	setCentralWidget(m_view);
	this->show();

}

Window::~Window()
{
	delete m_toolbar;
	delete m_testoption;
}

void Window::popup()
{
	QMessageBox popup;
	popup.setWindowTitle(tr("Popup!"));
	popup.setText("I'm distracting!");
	popup.exec();
	//Pretty much what it says on the tin.
}

void Window::place_rectangle()
{
}


