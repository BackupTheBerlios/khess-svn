INCLUDES = -I$(top_srcdir)/src/boards -I$(top_srcdir)/src/clocks \
	-I$(top_srcdir)/src/data -I$(top_srcdir)/src/game -I$(top_srcdir)/src/iointerfaces \
	-I$(top_srcdir)/src/lib -I$(top_srcdir)/src/modules -I$(top_srcdir)/src/modules/database \
	-I$(top_srcdir)/src/modules/internet -I$(top_srcdir)/src/parsers -I$(top_srcdir)/src/players \
	-I$(top_srcdir)/src/database $(all_includes)
METASOURCES = AUTO
noinst_LIBRARIES = libinternet.a
noinst_HEADERS = khinternetwidget.h khconsole.h khseektab.h khchatwindow.h \
		khconnectiondialog.h khtextbox.h khnicklist.h
libinternet_a_SOURCES = khinternetwidget.cpp khconsole.cpp khseektab.cpp \
			khchatwindow.cpp khconnectiondialog.cpp khtextbox.cpp khnicklist.cpp
