#-------------------------------------------------
#
# Project created by QtCreator 2012-06-18T18:37:15
#
#-------------------------------------------------

QT       += core gui

TARGET = OneCamera_JHcamDemo
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += ../../../SDK64
LIBS += ../../../SDK64/JHCap2.lib

#DEFINES +=DEBUG
SOURCES += main.cpp\
        mainwindow.cpp \   
        capturethread.cpp \
    iocontrol.cpp

HEADERS  += mainwindow.h \
            capturethread.h \
    iocontrol.h

FORMS    += mainwindow.ui \
    iocontrol.ui






