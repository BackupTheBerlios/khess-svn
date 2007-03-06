# Database core library

HEADERS +=  \
database.h \
databaseconversion.h \
databaseinfo.h \
engine.h \
filter.h \
filtermodel.h \
filtersearch.h \
index.h \
indexitem.h \
output.h \
outputoptions.h \
pgndatabase.h \
playerdata.h \
playerdatabase.h \
query.h \
search.h \
spellchecker.h \
tags.h \
tristatetree.h \
uciengine.h \
wbengine.h
SOURCES += database.cpp \
databaseconversion.cpp \
databaseinfo.cpp \
engine.cpp \
filter.cpp \
filtermodel.cpp \
filtersearch.cpp \
index.cpp \
indexitem.cpp \
output.cpp \
outputoptions.cpp \
pgndatabase.cpp \
playerdata.cpp \
playerdatabase.cpp \
query.cpp \
search.cpp \
spellchecker.cpp \
tags.cpp \
tristatetree.cpp \
uciengine.cpp \
wbengine.cpp
TARGET = database
CONFIG += staticlib
TEMPLATE = lib

include(../modules_config.pri)
