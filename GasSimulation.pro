#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T15:29:47
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

QMAKE_CXXFLAGS -=-O2
QMAKE_CXXFLAGS +=-O3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gazy
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    planet.cpp \
    data.cpp \
    vector2d.cpp \
    simwindow.cpp \
    simwidget.cpp \
    datawindow.cpp \
    datawidget.cpp \
    wall.cpp \
    wallwidget.cpp

HEADERS  += mainwindow.h \
    planet.h \
    data.h \
    vector2d.h \
    simwindow.h \
    simwidget.h \
    datawindow.h \
    datawidget.h \
    wall.h \
    wallwidget.h

FORMS    += mainwindow.ui \
    simwindow.ui \
    simwidget.ui \
    datawindow.ui \
    datawidget.ui \
    wallwidget.ui
