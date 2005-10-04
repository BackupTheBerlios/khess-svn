/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   David Cuadrado  krawek@gmail.com                                      *
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

#include "khinternetwidget.h"
#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>

#include <qvaluelist.h>

#include "khconnectiondialog.h"
#include "khchatwindow.h"
#include "khinternetwidget.h"
#include "khficsparser.h"

#include "khhtmlpage.h"


KHInternetWidget::KHInternetWidget(QWidget *parent) : QVBox(parent, "KHInternetWidget"), m_parser(0), m_chat(0)
{
	setCaption(i18n("Internet"));
	setIcon( SmallIcon("network"));
	m_tabBar = new KTabWidget(this);
	setupTabs();
}


KHInternetWidget::~KHInternetWidget()
{
}

void KHInternetWidget::setupTabs()
{
	m_console = new KHConsole(m_tabBar);
	m_tabBar->insertTab(m_console, i18n("Console"));
	
	m_seekTab = new KHSeekTab(m_tabBar);
	m_tabBar->insertTab(m_seekTab, i18n("Seek"));
}

void KHInternetWidget::closeConnection()
{
	if ( m_parser )
	{
		m_parser->closeResource();
	}
}

void KHInternetWidget::connectToServer()
{
	kdDebug() << "Connect" << endl;
	
	KHConnectionDialog *m_conDialog = new KHConnectionDialog(this);
	
	int result = m_conDialog->exec();
	
	if ( result == QDialog::Accepted )
	{
		kdDebug() << "Making connection..." << endl;
		
		ConnectionParam cp(m_conDialog->server(), m_conDialog->port().toInt(),m_conDialog->login(), m_conDialog->password() );
		cp.setGuest( m_conDialog->asGuest() );
				
		m_parser = new KHFicsParser(cp);
		
		connect(this, SIGNAL(sendMoveToServer(const KHChessMove &)), m_parser, SLOT(sendMove(const KHChessMove &)));
		
		connect(m_parser, SIGNAL(createMatch(KHMatch *)), this, SIGNAL(createNewMatch(KHMatch *)));
		connect(m_parser, SIGNAL(doFicsMove(KHFicsMove &)), this, SLOT(sendAMove(KHFicsMove &)));
		
		connect(m_parser, SIGNAL(sendToConsole(const QString &)), m_console, SLOT(setText(const QString &)));
		connect(m_console, SIGNAL(tryExec(const QString &)), m_parser, SLOT(execCommand(const QString &)));
		connect(m_parser, SIGNAL(sendShout(const QString &, const QString &)), this, SLOT(showShout(const QString &, const QString &)));
		
		connect(m_parser, SIGNAL(sendSeeks(const QValueList<QStringList> &)), m_seekTab, SLOT(pushSeeks(const QValueList<QStringList> &)));
			
		connect(m_seekTab, SIGNAL(iWantPlay(const QString &)), m_parser, SLOT(execCommand(const QString &)));
	}
	else if (result == KHConnectionDialog::Register )
	{
		sendPage("http://freechess.org/cgi-bin/Register/FICS_register.cgi", i18n("Register"));
	}
}

void KHInternetWidget::sendAMove(KHFicsMove &m)
{
	emit sendFicsMove(&m);
}

void KHInternetWidget::sendPage(const QString &url, const QString &title)
{
// 	KMdiChildView *view = new KMdiChildView(title);
// 	( new QVBoxLayout( view ) )->setAutoAdd( true );
// 	
// 	KHHtmlPage *page = new KHHtmlPage(view, "HTML_Page",  view, "HTML_Page" );
// 	
// 	page->openURL(url);
// 	
// 	emit showWebPage(view);
}

void KHInternetWidget::showChatWindow()
{
	if ( m_chat )
	{
		m_chat->show();
	}
	else
	{
		if ( m_parser && m_parser->haveConnection() )
		{
			m_chat = new KHChatWindow(this);
			m_chat->hide();
			connect(m_chat, SIGNAL(sendMessageToServer(const QString &, const QString &)), m_parser, SLOT(sendChatMessage(const QString &, const QString &)));
			m_chat->show();
			
			connect(m_chat, SIGNAL(sendToServer(const QString &)), m_parser, SLOT(execCommand(const QString &)));
			
			connect(m_parser, SIGNAL(sendToChat(const QString &, const QString &, const QString &)), m_chat, SLOT(setText(const QString &, const QString &, const QString &)) );
			
			connect(m_parser, SIGNAL(sendPrivateMessage(const QString &, const QString &)), m_chat, SLOT(setPrivateMessage(const QString &, const QString &) ));
			
			connect(m_parser, SIGNAL(sendChannelNicks(const QString &, const QStringList &)), m_chat, SLOT(putChannelNicks(const QString &, const QStringList &)));
			
			connect(m_parser, SIGNAL(sendChannels(const QValueList<QStringList> &)), m_chat, SLOT(pushChannels(const QValueList<QStringList> &)));
		}
		else
		{
			KMessageBox::error(this, i18n("Please connect to server first"), i18n("Error"));
		}
	}
}

void KHInternetWidget::showShout(const QString &from, const QString &msg)
{
	kdDebug() << "SHOW SHOUT" << endl;
	emit message2osd(from+" "+i18n("shouts")+" "+msg);
}



#include "khinternetwidget.moc"
