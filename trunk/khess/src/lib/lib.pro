TEMPLATE = lib

QT += gui xml core svg
CONFIG += staticlib \
warn_on
SOURCES += khapp.cpp
HEADERS += khapp.h
DLIB_DIR = ../dlib
include($$DLIB_DIR/dlib.pri)

