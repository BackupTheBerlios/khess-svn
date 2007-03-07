TEMPLATE = lib

CONFIG += staticlib \
warn_on
SOURCES += boarditem.cpp \
boardview.cpp \
pieceitem.cpp
HEADERS += boarditem.h \
boardview.h \
pieceitem.h

QT += svg

SOURCES_DIR = ..
include($$SOURCES_DIR/config.pri)

