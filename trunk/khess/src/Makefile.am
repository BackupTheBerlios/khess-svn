## Makefile.am for khess

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = khess

# set the include path for X, qt and KDE
INCLUDES = -I$(top_srcdir)/src/boards -I$(top_srcdir)/src/clocks \
	-I$(top_srcdir)/src/data -I$(top_srcdir)/src/dlslib -I$(top_srcdir)/src/game \
	-I$(top_srcdir)/src/iointerfaces -I$(top_srcdir)/src/lib -I$(top_srcdir)/src/modules \
	-I$(top_srcdir)/src/modules/database -I$(top_srcdir)/src/modules/internet -I$(top_srcdir)/src/parsers \
	-I$(top_srcdir)/src/players -I$(top_srcdir)/src/database $(all_includes)

# the library search path. 
khess_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
khess_LDADD = $(top_builddir)/src/dlslib/libdls.a \
	$(top_builddir)/src/modules/internet/libinternet.a $(top_builddir)/src/players/libplayers.a \
	$(top_builddir)/src/parsers/libparsers.a $(top_builddir)/src/boards/libboards.a \
	$(top_builddir)/src/clocks/libclocks.a $(top_builddir)/src/game/libgame.a \
	$(top_builddir)/src/iointerfaces/libiointerfaces.a $(top_builddir)/src/modules/database/libdatabase.a \
	$(top_builddir)/src/lib/libkh.a $(LIB_KHTML) $(LIB_KFILE) $(LIB_KDEPRINT) $(LIB_KDECORE)

# which sources should be compiled for khess
khess_SOURCES = main.cpp khess.cpp pref.cpp khessiface.skel khselectuser.cpp \
	khlogin.cpp osd.cpp khplayerinfobox.cpp khboardview.cpp khplayersstatus.cpp \
	khmatchdialog.cpp

# these are the headers for your project
noinst_HEADERS = khess.h pref.h khselectuser.h khlogin.h osd.h \
	khplayerinfobox.h khboardview.h khplayersstatus.h khmatchdialog.h

# client stuff

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/khess.pot

KDE_ICON = AUTO

# this is where the kdelnk file will go 
kdelnkdir = $(kde_appsdir)/Games/Board/
kdelnk_DATA = khess.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/khess
rc_DATA = khessui.rc


SUBDIRS = lib boards data clocks players game iointerfaces parsers modules \
	dlslib
