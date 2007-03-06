
TEMPLATE = app

SOURCES = main.cpp \
khess.cpp
HEADERS += khess.h

include(shell_config.pri)

CONFIG += warn_on

TARGET = khess.bin

DESTDIR = ../../bin/

