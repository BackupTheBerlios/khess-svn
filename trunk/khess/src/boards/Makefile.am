INCLUDES = -I$(top_srcdir)/src/clocks -I$(top_srcdir)/src/game \
	-I$(top_srcdir)/src/lib -I$(top_srcdir)/src/players $(all_includes)
METASOURCES = AUTO
lib_LIBRARIES = libboards.a
noinst_HEADERS = khboard2d.h khimagemanager.h khpromotiondialog.h khboardbase.h
libboards_a_SOURCES = khboard2d.cpp khimagemanager.cpp khpromotiondialog.cpp \
	khboardbase.cpp
boardsdir = $(kde_datadir)/khess/boards/
figuresdir = $(kde_datadir)/khess/figures
soundsdir = $(kde_datadir)/khess/sounds/
boards_DATA = khess-board.tar.gz
figures_DATA = khess-figures.tar.gz
