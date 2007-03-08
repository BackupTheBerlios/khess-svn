
TEMPLATE = app

SOURCES = main.cpp \
khess.cpp \
gamemanager.cpp
HEADERS += khess.h \
gamemanager.h
include(shell_config.pri)

CONFIG += warn_on

TARGET = khess.bin

DESTDIR = ../../bin/

