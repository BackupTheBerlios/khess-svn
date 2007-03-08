
TEMPLATE = app

SOURCES = main.cpp \
khess.cpp \
gamemanager.cpp \
engineinterface.cpp \
interfacefactory.cpp
HEADERS += khess.h \
gamemanager.h \
engineinterface.h \
interfacefactory.h
include(shell_config.pri)

CONFIG += warn_on

TARGET = khess.bin

DESTDIR = ../../bin/

