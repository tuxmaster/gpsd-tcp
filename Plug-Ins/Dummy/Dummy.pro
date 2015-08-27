include(../../Vorgaben.pri)

TEMPLATE 	= lib
CONFIG	       += plugin
VERSION		= 0.0.1
TARGET		= dummy
DESTDIR	        = ../
QT		= core


UI_DIR		= tmp/ui
MOC_DIR		= tmp/moc
OBJECTS_DIR	= tmp/obj
RCC_DIR		= tmp/rcc

HEADERS += \
    Quellen/Erweiterung_Dummy.h \
    Quellen/Dummy.h \
    Quellen/Vorgaben.h

SOURCES += \
    Quellen/Dummy.cpp


