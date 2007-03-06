/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado - krawek@gmail.com               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef _KHESS_H_
#define _KHESS_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qlabel.h>

#include <klocale.h>
#include <qlayout.h>
#include <kiconloader.h>
#include <klistbox.h>
#include <kstatusbar.h>
#include <kactionclasses.h>
#include <qlayout.h>
#include <kiconloader.h>
#include <qvaluelist.h>

#include <kprinter.h>
#include <qdict.h>

#include "osd.h"
#include "khboardview.h"

#include "khdatabasewidget.h"
#include "khinternetwidget.h"

#include "khlogin.h"
#include "khparserbase.h"
#include "khengineparser.h"
#include "khmatch.h"
#include "khpageview.h"

#include "dmainwindow.h"

class KPrinter;
class KURL;
class KHInternetIOInterface;
class KHChatWindow;
class KHFicsParser;
class KHEngineParser;

typedef QDict<KHBoardView> KHGames;

/**
 * This class serves as the main window for Khess.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author David Cuadrado - <krawek@gmail.com>
 * @version 0.1
 */

class Khess : public DMainWindow
{
	Q_OBJECT
	public:
		Khess();
		virtual ~Khess();
		void load(const KURL& url);

	protected:
		virtual void dragEnterEvent(QDragEnterEvent *event);
		virtual void dropEvent(QDropEvent *event);

	protected:
		void saveProperties(KConfig *);
		void readProperties(KConfig *);
		
		
	public slots:
		void showNotice(const QString &message);
		void sendMoveToBoard(KHChessMove *);


	private slots:
		void newLogin();
		void newPlayer();
		void newMatch();
		void connectToServer();
		void disconnectFromServer();
		void showChatWindow();
		
		void fileOpen();
		void fileSave();
		void fileSaveAs();
		void filePrint();
		void optionsPreferences();

		void changeStatusbar(const QString& text);
		void changeCaption(const QString& text);

		void createMatch(KHMatch *);

	private:
		void setupAccel();
		void setupActions();
		void setupToolWindows();
		void setupOSD();

	private:
		KHPageView *m_view;
		KPrinter   *m_printer;
		
		KHInternetWidget *m_internetWidget;
		KHDatabaseWidget *m_databaseWidget;
		
		KHLogin *m_login;
		
		OSDWidget *m_osd;
		
		KHGames m_games;
};

#endif // _KHESS_H_

