TEMPLATE = lib

CONFIG += staticlib \
warn_on
SOURCES += game.cpp \
figure.cpp \
position.cpp \
board.cpp \
common.cpp \
history.cpp \
historylist.cpp \
move.cpp \
nag.cpp \
partialdate.cpp \
settings.cpp
HEADERS += game.h \
figure.h \
position.h \
board.h \
common.h \
history.h \
historylist.h \
move.h \
movelist.h \
nag.h \
partialdate.h \
rand64.h \
settings.h
