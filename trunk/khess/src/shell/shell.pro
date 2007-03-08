
TEMPLATE = app

SOURCES = main.cpp \
khess.cpp \
gamemanager.cpp \
interfacefactory.cpp \
matchdialog.cpp
HEADERS += khess.h \
gamemanager.h \
matchdialog.h
include(shell_config.pri)

CONFIG += warn_on

TARGET = khess.bin

DESTDIR = ../../bin/

SOURCES -= engineinterface.cpp \
interfacefactory.cpp
