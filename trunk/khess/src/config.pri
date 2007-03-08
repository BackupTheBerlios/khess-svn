
INCLUDEPATH += $$SOURCES_DIR

LIBS += -L$$SOURCES_DIR/game -lgame -L$$SOURCES_DIR/board -lboard -L$$SOURCES_DIR/interfaces -linterfaces 

include($$SOURCES_DIR/modules/modules.pri)

linux-g++ {
TARGETDEPS += $$SOURCES_DIR/game/libgame.a $$SOURCES_DIR/board/libboard.a $$SOURCES_DIR/interfaces/libinterfaces.a
}

