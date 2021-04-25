#include "StartWindow.h"

StartWindow::StartWindow()
{
    m_name = new QLabel(tr("Spring 2021 CMSC461 Project"));
    m_join_canvas = new QPushButton(tr("Join a canvas"));
    m_load_canvas = new QPushButton(tr("Load canvas from file"));
    m_create_canvas = new QPushButton(tr("Create new canvas"));
    m_quit = new QPushButton(tr("Quit"));

    m_layout = new QVBoxLayout;
    m_layout->addWidget(m_name);
    m_layout->addWidget(m_join_canvas);
    m_layout->addWidget(m_create_canvas);
    m_layout->addWidget(m_load_canvas);
    m_layout->addWidget(m_quit);

    setLayout(m_layout);
}

StartWindow::~StartWindow()
{
    delete m_name;
    delete m_join_canvas;
    delete m_load_canvas;
    delete m_create_canvas;
    delete m_quit;
    delete m_layout;
}
