include(../../Vorgaben.pri)

TEMPLATE 	= lib
CONFIG	       += plugin
VERSION		= 0.0.1
TARGET		= em7345
DESTDIR	        = ../
QT		= core serialport


UI_DIR		= tmp/ui
MOC_DIR		= tmp/moc
OBJECTS_DIR	= tmp/obj
RCC_DIR		= tmp/rcc

HEADERS += \
    Quellen/Erweiterung_EM7345.h \
    Quellen/EM7345.h \
    Quellen/Vorgaben.h

SOURCES += \
    Quellen/EM7345.cpp


