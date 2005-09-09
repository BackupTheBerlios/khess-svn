INCLUDES = -I$(top_srcdir)/src/boards -I$(top_srcdir)/src/clocks \
	-I$(top_srcdir)/src/data -I$(top_srcdir)/src/iointerfaces -I$(top_srcdir)/src/lib \
	-I$(top_srcdir)/src/modules -I$(top_srcdir)/src/modules/database \
	-I$(top_srcdir)/src/modules/internet -I$(top_srcdir)/src/parsers -I$(top_srcdir)/src/players $(all_includes)
METASOURCES = AUTO
lib_LIBRARIES = libgame.a
libgame_a_SOURCES = figure.cpp khgame.cpp position.cpp khchessmove.cpp \
	khmatch.cpp khficsmove.cpp
noinst_HEADERS = figure.h khgame.h position.h khchessmove.h khmatch.h \
	khficsmove.h
