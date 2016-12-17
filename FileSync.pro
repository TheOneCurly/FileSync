#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T18:36:27
#
#-------------------------------------------------

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileSync
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    host.cpp \
    client.cpp

HEADERS  += mainwindow.h\
    host.h \
    client.h

FORMS    += mainwindow.ui

#INCLUDEPATH += /usr/include

unix:!macx: INCLUDEPATH += /usr/include
win32: INCLUDEPATH += /usr/lib/mxe/usr/i686-w64-mingw32.static/include
