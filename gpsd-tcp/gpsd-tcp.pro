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
    Quellen/Haupt.cpp \
    Quellen/Steuerung.cpp

HEADERS += \
    Quellen/Vorgaben.h \
    Quellen/Meldung.h \
    Quellen/Steuerung.h \
    Quellen/Plugin.h \
    Quellen/Pluginfabrik.h

TRANSLATIONS    =  Uebersetzungen/gpsd-tcp_en.ts

QMAKE_EXTRA_COMPILERS += lrelease
lrelease.input         = TRANSLATIONS
lrelease.output        = ${QMAKE_FILE_BASE}.qm
lrelease.commands      = $$[QT_INSTALL_BINS]/lrelease-qt5 ${QMAKE_FILE_IN} -qm Uebersetzungen/${QMAKE_FILE_BASE}.qm
lrelease.CONFIG       += no_link target_predeps

Dokumentation.commands = doxygen
QMAKE_EXTRA_TARGETS += Dokumentation
