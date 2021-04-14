QT += testlib
QT += network
QT -= gui
QT += core gui
QT += widgets
QT += sql

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testserver.cpp
