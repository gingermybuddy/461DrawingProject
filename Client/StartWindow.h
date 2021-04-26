#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>


class StartWindow : public QWidget
{
    Q_OBJECT
    public:
        QLabel* m_name;
        QPushButton* m_join_canvas;
        QPushButton* m_create_canvas;
        QPushButton* m_load_canvas;
        QPushButton* m_quit;
        QVBoxLayout* m_layout;
        StartWindow();
        ~StartWindow();
};

#endif // STARTWINDOW_H
