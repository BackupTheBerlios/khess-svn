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


#include "khess.h"
#include "pref.h"

#include <qdragobject.h>
#include <kprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>

#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kdeversion.h>
#include <kstatusbar.h>
#include <kaccel.h>
#include <kio/netaccess.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <kurldrag.h>
#include <kurlrequesterdlg.h>

#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>
///////////////
#include <qdragobject.h>
#include <kprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>

#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kdeversion.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kkeydialog.h>
#include <kaccel.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <ktabwidget.h>
#include <ktoolbarbutton.h>
#include <kapplication.h>

#include <kedittoolbar.h>
#include <kdebug.h>
#include <qobjectlist.h>

#include "khnewplayer.h"

#include "khficsparser.h"
#include "khengineparser.h"

#include "khplayersstatus.h"
#include "khmatchdialog.h"

#include "khwebbrowser.h"

Khess::Khess() : KMdiMainFrm( 0, "Khess-main", KMdi::IDEAlMode ), m_view(0), m_printer(0),m_internetWidget(0), m_databaseWidget(0), m_login(0)
{
	setAcceptDrops(true);
	
	setMinimumSize( 800, 715);

	setupActions();
	createGUI(0);
	
	statusBar()->show();
	setupOSD();
	
	m_login = new KHLogin(this);
	show();
	newLogin();
	
	QToolButton *closeCurrentTab = new QToolButton(tabWidget()); // avoid kdelibs 3.4 bug
	closeCurrentTab->setIconSet(SmallIcon("tab_remove"));
	
	tabWidget()->setCornerWidget( closeCurrentTab, KToolBarButton::TopRight);
	tabWidget()->show();
	connect(closeCurrentTab, SIGNAL(clicked()), SLOT(closeCurrent()));
	
	setTabWidgetVisibility( KMdi::AlwaysShowTabs);
	
	KHWebBrowser *browser = new KHWebBrowser(this);
	addWindow(browser);
}

Khess::~Khess()
{
	kdDebug() << "[Destroying MainWindow]" << endl;
	
	if (m_login)
		delete m_login;
	
	while( m_pCurrentWindow )
	{
		closeCurrent();
	}
}

void Khess::setupOSD()
{
	m_osd = new OSDWidget("Khess", this, "OSD");
	m_osd->setDuration( 2000 );
	m_osd->setEnabled( true );
	//osd->setOffset( 400,400 );
	m_osd->setAlignment( OSDWidget::Right );
	m_osd->setScreen( 0 );
	m_osd->setShadow( true );
	m_osd->setBackgroundColor( 0x4e438a );
	m_osd->setTextColor(0xcdcd53);
	m_osd->setFont( QFont("Arial", 20) );
}

void Khess::setupToolWindows()
{
	if ( m_view )
	{
		closeWindow(m_view);
	}
	
	m_view = new KHPageView(i18n("Welcome"), this);
	addWindow(m_view);
	
	// DATABASE
	m_databaseWidget = new KHDatabaseWidget(this);
	m_toolWindows << addToolWindow( m_databaseWidget, KDockWidget::DockLeft, getMainDockWidget() );
	
	connect(m_databaseWidget, SIGNAL(message2osd(const QString &)), this, SLOT(showNotice(const QString &)));
	//////////////////
	
	// INTERNET
	m_internetWidget = new KHInternetWidget(this);
	m_toolWindows << addToolWindow(m_internetWidget, KDockWidget::DockLeft, getMainDockWidget());
	connect(m_internetWidget, SIGNAL(message2osd(const QString &)), this, SLOT(showNotice(const QString &)));
	connect(m_internetWidget, SIGNAL(createNewMatch(KHMatch *)), this, SLOT(createMatch(KHMatch *)));
	connect(m_internetWidget, SIGNAL(showWebPage(KMdiChildView *)), this, SLOT(addWindow(KMdiChildView *)));
	
	connect(m_internetWidget, SIGNAL(sendFicsMove(KHChessMove *)), this, SLOT(sendMoveToBoard(KHChessMove *)));
	
	/////////////////////
}

void Khess::sendMoveToBoard(KHChessMove *move)
{
	QString gameId = move->gameId();
	
	KHBoardView *board = m_games[gameId];
	if ( board )
	{
		board->doMove(move);
	}
	else
	{
		kdDebug() << "Juego no encontrado: " << gameId << endl;
	}
}

void Khess::load(const KURL& url)
{
	QString target;
    // the below code is what you should normally do.  in this
    // example case, we want the url to our own.  you probably
    // want to use this code instead for your app

#if 0
    // download the contents
    if (KIO::NetAccess::download(url, target))
{
        // set our caption
	setCaption(url);

        // load in the file (target is always local)
	loadFile(target);

        // and remove the temp file
	KIO::NetAccess::removeTempFile(target);
}
#endif

    setCaption(url.prettyURL());
}

void Khess::setupActions()
{
	KStdAction::openNew(this, SLOT(newLogin()), actionCollection());
	KStdAction::open(this, SLOT(fileOpen()), actionCollection());
	KStdAction::save(this, SLOT(fileSave()), actionCollection());
	KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
	KStdAction::print(this, SLOT(filePrint()), actionCollection());
	
	KStdAction::quit(this, SLOT(close()), actionCollection());

	KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

	new KAction(i18n("New player"), 0, this, SLOT(newPlayer()), actionCollection(), "new_player");
	
	new KAction(i18n("New match"), 0, this, SLOT(newMatch()), actionCollection(), "new_match");
	
	new KAction(i18n("Offer draw"), 0, this, SLOT(newMatch()), actionCollection(), "offer_draw");
	
	new KAction(i18n("Connect to server..."), 0, this, SLOT(connectToServer()), actionCollection(), "connect");
	
	new KAction(i18n("Chat window..."), 0, this, SLOT(showChatWindow()), actionCollection(), "open_chat");
	
	new KAction(i18n("Disconnect from server..."), 0, this, SLOT(disconnectFromServer()), actionCollection(), "disconnect");
}

void Khess::saveProperties(KConfig *config)
{
}

void Khess::readProperties(KConfig *config)
{
}

void Khess::dragEnterEvent(QDragEnterEvent *event)
{
    // accept uri drops only
	event->accept(KURLDrag::canDecode(event));
}

void Khess::dropEvent(QDropEvent *event)
{
}

void Khess::closeCurrent()
{
	// If there's a current view, close it
	if ( m_pCurrentWindow != 0 )
	{
		KHBoardView *boardtmp = dynamic_cast<KHBoardView *>(m_pCurrentWindow);
		
		if ( boardtmp )
		{
			if (boardtmp->playerStatus())
			{
				deleteToolWindow(boardtmp->playerStatus());
			}
		}
		
		closeWindow( m_pCurrentWindow );
	}
}

void Khess::newLogin()
{
	kdDebug() << "New login" << endl;

	m_login->show();
	
	int result = m_login->exec() ;
	
	if ( result == QDialog::Accepted )
	{
		setupToolWindows();
	}
	else if ( result == 123 )
	{
		newPlayer();
	}
}

void Khess::fileOpen()
{
}

void Khess::fileSave()
{
}

void Khess::fileSaveAs()
{
}

void Khess::filePrint()
{
    // this slot is called whenever the File->Print menu is selected,
    // the Print shortcut is pressed (usually CTRL+P) or the Print toolbar
    // button is clicked
	if (!m_printer) m_printer = new KPrinter;
	if (m_printer->setup(this))
	{
        // setup the printer.  with Qt, you always "print" to a
        // QPainter.. whether the output medium is a pixmap, a screen,
        // or paper
		QPainter p;
		p.begin(m_printer);

        // we let our view do the actual printing
		QPaintDeviceMetrics metrics(m_printer);
// 		m_view->print(&p, metrics.height(), metrics.width());

        // and send the result to the printer
		p.end();
	}
}

void Khess::optionsPreferences()
{
    // popup some sort of preference dialog, here
	KhessPreferences dlg;
	if (dlg.exec())
	{
        // redo your settings
	}
}

void Khess::changeStatusbar(const QString& text)
{
    // display the text on the statusbar
	statusBar()->message(text);
}

void Khess::changeCaption(const QString& text)
{
    // display the text on the caption
	setCaption(text);
}

// PLAYERS

void Khess::newPlayer()
{
	kdDebug() << "New player" << endl;
	KHNewPlayer *newPlayer = new KHNewPlayer(this);
	if ( newPlayer->exec() == QDialog::Accepted )
	{
		showNotice(i18n("Player created"));
	}
}

// MATCH

void Khess::newMatch()
{
	kdDebug() << "New match" << endl;
	
	KHMatchDialog matchDialog(this);
	
	if ( matchDialog.exec() != QDialog::Rejected )
	{
		createMatch(matchDialog.match());
	}
}

void Khess::createMatch(KHMatch *match)
{
	KHBoardView *board = new KHBoardView(i18n("Game"), this, match );
	addWindow(board);
	connect(board, SIGNAL(signalChangeStatusbar(const QString&)), this,   SLOT(changeStatusbar(const QString&)));
	connect(board, SIGNAL(signalChangeCaption(const QString&)),this,   SLOT(changeCaption(const QString&)));
	
	QString gameid = "";

	if ( match )
	{
		switch(match->type())
		{
			case KHMatch::INTERNET:
			{
				connect(board, SIGNAL(moved(const KHChessMove &)), m_internetWidget, SIGNAL(sendMoveToServer(const KHChessMove &)));
			}
			break;
			case KHMatch::LOCAL:
			{
			}
			break;
			case KHMatch::ENGINE:
			{
				KHEngineParser *m_engine = new KHEngineParser(match);

				connect(board, SIGNAL(moved(const KHChessMove &)), m_engine, SLOT(sendMoveToEngine(const KHChessMove &)));
				
				connect(m_engine, SIGNAL(doMove(KHChessMove *)), this, SLOT( sendMoveToBoard(KHChessMove *) ));
				
			}
			break;
		}
		
		KHPlayersStatus *m_playersView = new KHPlayersStatus(match, this, match->id());

		m_toolWindows << addToolWindow( m_playersView, KDockWidget::DockRight, getMainDockWidget() );
		
		board->setCaption( match->white()->login()+ "("+QString::number(match->white()->elo())+ ")" + " vs " + match->black()->login()+"("+QString::number(match->black()->elo())+")"  );
		
		board->setPlayerStatus( m_playersView );
		
		gameid = match->id();
	}
	
	m_games.insert( gameid , board);
}


// INTERNET

void Khess::connectToServer()
{
	if ( m_internetWidget )
	{
		m_internetWidget->connectToServer();
	}
	else
	{
		kdError() << "Please first log in the system" << endl;
	}
}

void Khess::disconnectFromServer()
{
	kdDebug() << "Disconnect" << endl;
	if ( m_internetWidget )
	{
		m_internetWidget->closeConnection();
	}
	else
	{
		kdError() << "Please first log in the system" << endl;
	}
}

void Khess::showChatWindow()
{
	if ( m_internetWidget )
	{
		m_internetWidget->showChatWindow();
	}
	else
	{
		kdError() << "Please first log in the system" << endl;
	}
}

//----------

void Khess::showNotice(const QString &message)
{
	kdDebug() << "NOTICE: " << message << endl;
	m_osd->setDuration( message.length() * 80 ); // 70 is perfect
	m_osd->show(message, false, false);
}

#include "khess.moc"
