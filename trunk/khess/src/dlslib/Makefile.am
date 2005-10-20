INCLUDES = -I$(top_srcdir)/src/boards -I$(top_srcdir)/src/clocks \
	-I$(top_srcdir)/src/data -I$(top_srcdir)/src/game -I$(top_srcdir)/src/iointerfaces \
	-I$(top_srcdir)/src/lib -I$(top_srcdir)/src/modules -I$(top_srcdir)/src/modules/database \
	-I$(top_srcdir)/src/modules/internet -I$(top_srcdir)/src/parsers -I$(top_srcdir)/src/players \
	-I$(top_srcdir)/src/dlslib -I$(top_srcdir)/src/database $(all_includes)
METASOURCES = AUTO
noinst_LIBRARIES = libdls.a
noinst_HEADERS = buttonbar.h button.h clicklineedit.h comdefs.h ddockwindow.h\
	dlistwidget.h dmainwindow.h docksplitter.h dtable.h dtabwidget.h\
	dwidgetbase.h
libdls_a_SOURCES = buttonbar.cpp button.cpp clicklineedit.cpp ddockwindow.cpp\
	dlistwidget.cpp dmainwindow.cpp docksplitter.cpp dtable.cpp\
	dtabwidget.cpp dwidgetbase.cpp
noinst_DATA = close.xpm erase.xpm
AM_CXXFLAGS = -DUSE_KDE
