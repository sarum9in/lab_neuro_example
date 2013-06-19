#-------------------------------------------------
#
# Project created by QtCreator 2013-06-19T05:14:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Gui
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.hpp

FORMS    += MainWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../neuro/release/ -lneuro
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../neuro/debug/ -lneuro
else:unix: LIBS += -L$$OUT_PWD/../neuro/ -lneuro

INCLUDEPATH += $$PWD/../neuro
DEPENDPATH += $$PWD/../neuro
