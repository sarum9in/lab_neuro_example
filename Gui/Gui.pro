#-------------------------------------------------
#
# Project created by QtCreator 2013-06-19T05:14:39
#
#-------------------------------------------------

QT       += core gui script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 qwt

TARGET = Gui
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    GridField.cpp \
    RecognitionController.cpp \
    RecognitionDialog.cpp \
    ReadOnlyCheckBox.cpp \
    BooleanController.cpp \
    FunctionController.cpp

HEADERS  += MainWindow.hpp \
    GridField.hpp \
    RecognitionController.hpp \
    RecognitionDialog.hpp \
    ReadOnlyCheckBox.hpp \
    BooleanController.hpp \
    FunctionController.hpp

FORMS    += MainWindow.ui \
    RecognitionDialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../neuro/release/ -lneuro
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../neuro/debug/ -lneuro
else:unix: LIBS += -L$$OUT_PWD/../neuro/ -lneuro

INCLUDEPATH += $$PWD/../neuro
DEPENDPATH += $$PWD/../neuro
