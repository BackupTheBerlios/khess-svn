TEMPLATE = lib

CONFIG += staticlib \
warn_on
SOURCES += engineinterface.cpp \
interface.cpp \
interfacefactory.cpp
HEADERS += engineinterface.h \
interface.h \
interfacefactory.h

SOURCES_DIR = ..

include($$SOURCES_DIR/config.pri)

