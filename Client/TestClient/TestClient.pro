QT += testlib
QT += gui
QT += network
QT += core gui
QT += widgets
QT += sql
QT += gui widgets

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

HEADERS += #../ProjectView.h ../ProjectScene.h ../toolBar.h ../itemStats.h ../window.h

SOURCES +=  \
    TestClient.cpp
