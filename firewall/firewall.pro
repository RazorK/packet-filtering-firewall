#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T12:36:06
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = firewall
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editwindow.cpp

HEADERS  += mainwindow.h \
    connection.h \
    editwindow.h

FORMS    += mainwindow.ui \
    editwindow.ui
