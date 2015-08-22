include(../Vorgaben.pri)

TEMPLATE        = app
TARGET		= gpsd-tcp
CONFIG 	       += link_pkgconfig
QT		= core network

PKGCONFIG      += libsystemd-journal

MOC_DIR		= tmp/moc
OBJECTS_DIR	= tmp/obj
RCC_DIR		= tmp/rcc

SOURCES += \
    Quellen/Haupt.cpp

HEADERS += \
    Quellen/Vorgaben.h
