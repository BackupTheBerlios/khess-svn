TEMPLATE = lib

CONFIG += staticlib \
warn_on
SOURCES += engineinterface.cpp \
interface.cpp \
interfacefactory.cpp \
internetinterface.cpp
HEADERS += engineinterface.h \
interface.h \
interfacefactory.h \
internetinterface.h
SOURCES_DIR = ..

include($$SOURCES_DIR/config.pri)

QT += network

