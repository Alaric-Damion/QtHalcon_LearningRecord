#-------------------------------------------------
#
# Project created by QtCreator 2021-10-15T09:50:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CameraDllTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp

HEADERS += \
        widget.h

FORMS += \
        widget.ui

#Basler相机
INCLUDEPATH   += $$(BASLERROOT)/include
LIBS   += -L$$(BASLERROOT)/lib/x64

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Balser_GigeCamera/release/ -lBalser_GigeCamera
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Balser_GigeCamera/debug/ -lBalser_GigeCamera

#INCLUDEPATH += $$PWD/../Balser_GigeCamera/debug
#DEPENDPATH += $$PWD/../Balser_GigeCamera/debug


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lBalser_GigeCamera
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lBalser_GigeCamerad

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
