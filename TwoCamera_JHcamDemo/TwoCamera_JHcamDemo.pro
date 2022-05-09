#-------------------------------------------------
#
# Project created by QtCreator 2011-05-25T10:53:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TwoCamera_JHcamDemo
TEMPLATE = app

INCLUDEPATH += ../../../SDK
LIBS += ../../../SDK/JHCap2.lib

#DEFINES +=DEBUG
TRANSLATIONS=usbVideo_zh_CN.ts
SOURCES += main.cpp \
        mainwindow.cpp \
    aboutdialog.cpp \
    settingdialog.cpp \
    capturethread.cpp \
    histogram.cpp

HEADERS  += \
    mainwindow.h \
    aboutdialog.h \
    settingdialog.h \
    capturethread.h \
    histogram.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    settingdialog.ui \
    histogram.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc

RC_FILE = app.rc





























