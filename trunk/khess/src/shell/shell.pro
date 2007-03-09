
TEMPLATE = app

SOURCES = main.cpp \
khess.cpp \
gamemanager.cpp \
interfacefactory.cpp \
matchdialog.cpp \
connectdialog.cpp
HEADERS += khess.h \
gamemanager.h \
matchdialog.h \
connectdialog.h
include(shell_config.pri)

CONFIG += warn_on

TARGET = khess.bin

DESTDIR = ../../bin/

SOURCES -= engineinterface.cpp \
interfacefactory.cpp
