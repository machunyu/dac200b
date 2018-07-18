#-------------------------------------------------
#
# Project created by QtCreator 2018-02-12T22:51:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = showPics
TEMPLATE = app


MOC_DIR=x86/moc
RCC_DIR=x86/rcc
UI_DIR=x86/ui
OBJECTS_DIR=x86/obj

SOURCES += main.cpp\
        showpicsdlg.cpp

HEADERS  += showpicsdlg.h

FORMS    += showpicsdlg.ui

RESOURCES += image.qrc
