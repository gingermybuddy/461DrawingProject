#include "window.h"
#include "qcolordialog.h"
#include <QMessageBox>
#include <QLayout>
#include <QColorDialog>
#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <iostream>

Window::Window() : QMainWindow()
{
	//Anything new you add to the window is going to be a pointer to whatever object you're throwing in.
	//This sets up a toolbar inside the mainwindow with the 'this' call.
    show_start();
}

void Window::setup_window()
{
    m_scene = new ProjectScene();
    m_scene->setBackgroundBrush(Qt::white);
    m_view = new ProjectView; //Custom graphics scene where we can overload stuff
    m_view->change_tool(1); // set default tool to line
    m_view->change_color(255,0,0); // set default color to black
    m_view->setScene(m_scene);

    m_menu_bar = new QToolBar(this);
    m_save_img = new QAction("Save to Image");
    m_save_canvas = new QAction("Save Canvas");
    m_menu_bar->addAction(m_save_canvas);
    m_menu_bar->addAction(m_save_img);
    m_menu_bar->addSeparator();
    addToolBar(m_menu_bar);

    m_bar = new ToolBar();
    m_bar->set_view(m_view);
    m_tool_dock = new QDockWidget();
    m_tool_dock->setWidget(m_bar);

    m_tool_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    connect(m_scene, SIGNAL(changed(const QList<QRectF>&)), m_scene, SLOT(sceneChanged(const QList<QRectF>&)));
    connect(m_save_img, SIGNAL(triggered()), m_view, SLOT(saveToImage()));
    connect(m_save_canvas, SIGNAL(triggered()), m_view, SLOT(saveCanvas()));
    setCentralWidget(m_view);
    addDockWidget(Qt::LeftDockWidgetArea, m_tool_dock);
}

void Window::show_start()
{
    m_start_menu = new StartWindow();
    connect(m_start_menu->m_join_canvas, SIGNAL(clicked()), this, SLOT(join_canvas()));
    connect(m_start_menu->m_load_canvas, SIGNAL(clicked()), this, SLOT(load_canvas()));
    connect(m_start_menu->m_create_canvas, SIGNAL(clicked()), this, SLOT(create_canvas()));
    connect(m_start_menu->m_quit, SIGNAL(clicked()), m_start_menu, SLOT(close()));
    m_start_menu->show();
}

Window::~Window()
{
	// delete m_testoption;
}

void Window::join_canvas()
{
    QDialog params_window(this);
    QFormLayout submit(&params_window);
    QLineEdit* ip = new QLineEdit(&params_window);
    QLineEdit* port = new QLineEdit(&params_window);
    QLineEdit* board_id = new QLineEdit(&params_window);

    submit.addRow(tr("IP:"), ip);
    submit.addRow(tr("Port:"), port);
    submit.addRow(tr("Board ID:"), board_id);
    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &params_window);
    submit.addRow(&buttons);
    QObject::connect(&buttons, SIGNAL(accepted()), &params_window, SLOT(accept()));
    QObject::connect(&buttons, SIGNAL(rejected()), &params_window, SLOT(reject()));

    if(params_window.exec() == QDialog::Accepted) {
        setup_window();
        QHostAddress host;
        if(ip->text() == "localhost") {
            host = QHostAddress::LocalHost;
        } else {
            host = QHostAddress(ip->text());
        }
        if(!m_scene->connectToBoard(host, port->text().toInt(), board_id->text().toStdString())) {
            delete m_scene;
            delete m_view;
            delete m_bar;
            delete m_tool_dock;
            QMessageBox error;
            error.setWindowTitle("Failed");
            error.setText("Could not connect to the specified board. Check your connection.");
            error.exec();
        } else {
            m_start_menu->close();
            this->show();
        }
    }
    delete ip;
    delete port;
    delete board_id;
}

void Window::load_canvas()
{

}

void Window::create_canvas()
{
    QDialog params_window(this);
    QFormLayout submit(&params_window);
    QLineEdit* ip = new QLineEdit(&params_window);
    QLineEdit* port = new QLineEdit(&params_window);
    QLineEdit* board_id = new QLineEdit(&params_window);

    submit.addRow(tr("IP:"), ip);
    submit.addRow(tr("Port:"), port);
    submit.addRow(tr("Board ID:"), board_id);
    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &params_window);
    submit.addRow(&buttons);
    QObject::connect(&buttons, SIGNAL(accepted()), &params_window, SLOT(accept()));
    QObject::connect(&buttons, SIGNAL(rejected()), &params_window, SLOT(reject()));

    if(params_window.exec() == QDialog::Accepted) {
        setup_window();
        QHostAddress host;
        if(ip->text() == "localhost") {
            host = QHostAddress::LocalHost;
        } else {
            host = QHostAddress(ip->text());
        }
        if(!m_scene->connectToBoard(host, port->text().toInt(), board_id->text().toStdString())) {
            QDialog error(this);
            error.setWindowTitle("Failed");
            QFormLayout errorlayout(&error);
            QLabel* errormsg = new QLabel("Could not connect to the server. Would you like to create this canvas in offline mode?");
            errorlayout.addRow(errormsg);
            QDialogButtonBox ebuttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &error);
            errorlayout.addRow(&ebuttons);
            QObject::connect(&ebuttons, SIGNAL(accepted()), &error, SLOT(accept()));
            QObject::connect(&ebuttons, SIGNAL(rejected()), &error, SLOT(reject()));

            if(error.exec() == QDialog::Accepted) {
                m_start_menu->close();
                this->show();
            } else {
                delete m_scene;
                delete m_view;
                delete m_bar;
                delete m_tool_dock;
            }
        } else {
            m_start_menu->close();
            this->show();
        }
    }
    delete ip;
    delete port;
    delete board_id;
}

ToolBar* Window::get_m_bar()
{
    return m_bar;
}
QDockWidget* Window::get_m_tool_dock()
{
    return m_tool_dock;
}
ProjectScene* Window::get_m_scene()
{
    return m_scene;
}
ProjectView* Window::get_m_view()
{
    return m_view;
}

void Window::popup()
{
	QMessageBox popup;
	popup.setWindowTitle(tr("Popup!"));
	popup.setText("I'm distracting!");
	popup.exec();
	//Pretty much what it says on the tin.
}

