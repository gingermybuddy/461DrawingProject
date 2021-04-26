QT += testlib
QT += gui
QT += network
QT += core gui
QT += widgets
QT += sql
QT += gui widgets

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

INCLUDEPATH += \
                                ../

#LIBS += \-L../461DrawingProject \
           #-l461DrawingProject

HEADERS += ../ProjectView.h ../ProjectScene.h ../toolBar.h ../itemStats.h ../window.h ../StartWindow.h

SOURCES += TestClient.cpp ../ProjectView.cpp ../ProjectScene.cpp ../toolBar.cpp ../itemStats.cpp ../window.cpp ../StartWindow.cpp
