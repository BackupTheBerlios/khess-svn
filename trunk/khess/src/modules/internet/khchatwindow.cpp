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
 
#include "khchatwindow.h"

#include <qpushbutton.h>

#include <klocale.h>
#include <kdebug.h>
#include <kseparator.h>
#include <kiconloader.h>
#include <ktoolbarbutton.h>
#include <kmessagebox.h>

#include <qlabel.h>
#include <qregexp.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qtimer.h>


#include "khwarrior.h"

KHChatWindow::KHChatWindow(QWidget *parent)
	: KDialog(parent, "KHChatWindow")
{
	setCaption(i18n("Chat window"));
	
	(new QVBoxLayout(this))->setAutoAdd(true);
	
// 	setButtonCancel(KGuiItem(i18n("Close"), QString::null, i18n("Click for close the chat window"), i18n("Close this dialog")));
	
// 	connect(actionButton(Cancel), SIGNAL(clicked()), this, SLOT(hide()));
	
	
	m_tabWidget = new KTabWidget(this);
	m_tabWidget->setMinimumWidth(400);
			
	QToolButton *closeCurrentTab = new QToolButton(m_tabWidget); // avoid kdelibs 3.4 bug
	closeCurrentTab->setIconSet(SmallIcon("tab_remove"));
	
	m_tabWidget->setCornerWidget( closeCurrentTab, KToolBarButton::TopRight);
	m_tabWidget->show();
	connect(closeCurrentTab, SIGNAL(clicked()), SLOT(closeCurrent()));
	
	m_mainPage = createPage(i18n("Channel list"), i18n("Choose the channel"));
	
	m_listView = new KListView(m_mainPage);
	m_listView->addColumn(i18n("Channel #"));
	m_listView->addColumn(i18n("Name"));
	
	m_listView->setRenameable (1);
	
	m_menu = new KPopupMenu(i18n("Chat"), this);
	m_menu->insertItem(i18n("Enter to channel"),0);
	m_menu->insertItem(i18n("Edit name"),1 );
	connect(m_menu, SIGNAL(activated(int)), this, SLOT(menuActivated(int)));
	
	connect(m_listView, SIGNAL(contextMenu (KListView *, QListViewItem *, const QPoint &)), this, SLOT(showMenu(KListView *, QListViewItem *, const QPoint &)));
	
	connect(m_listView, SIGNAL(executed(QListViewItem *)), this, SLOT(enterToChannel(QListViewItem *)));
	
	new QLabel(i18n("Add channel"), m_mainPage);
	m_addChannel = new KLineEdit(m_mainPage);
	connect(m_addChannel, SIGNAL(returnPressed(const QString &)), this, SLOT(addChannel(const QString &)));
	
	m_channels << "1" << "50";
	addChannel("1", i18n("Help"));
	addChannel("50", i18n("General"));
	
	setModal(false);
	
	setMinimumSize(500, 400);
	
}

KHChatWindow::~KHChatWindow()
{
}

void KHChatWindow::chatWith(const QString &nick)
{
	QVBox *box = createPage(nick, i18n("Chat with %1").arg(nick));
	
	KHChatWindowPrivate *tmpChat = new KHChatWindowPrivate(box, nick);
	connect(tmpChat, SIGNAL(sendMessageToServer(const QString &, const QString &)), SIGNAL(sendMessageToServer(const QString &, const QString &)));
	
	m_chats.insert(nick,tmpChat );
}

void KHChatWindow::pushChannels(const QValueList<QStringList> &chans)
{
	for(uint i = 0; i < chans.count(); i++)
	{
		QStringList aChan = chans[i];
		QStringList::iterator it;
		
		KListViewItem *item = new KListViewItem(m_listView);
		
		int position = 0;
		for(it = aChan.begin(); it != aChan.end(); ++it)
		{
			item->setText(position, *it);
			position++;
		}
	}
}

void KHChatWindow::showMenu(KListView *l, QListViewItem *i, const QPoint &p)
{
	if ( i )
	{
		m_menu->popup(p);
	}
}

void KHChatWindow::menuActivated(int id)
{
	switch(id)
	{
		case 0:
		{
			enterToChannel(m_listView->currentItem());
		}
		break;
		case 1:
		{
			m_listView->rename(m_listView->currentItem(),1);
		}
		break;
	}
}


QListViewItem * KHChatWindow::addChannel(const QString &id, const QString &name)
{
	KListViewItem *ctemp = new KListViewItem(m_listView);
	ctemp->setText(0, id);
	ctemp->setText(1, name);
	m_listView->insertItem(ctemp);
	
	bool ok = false;
	id.toInt(&ok);
	if (ok) // channel
	{
		// +channel name
		emit sendToServer("+channel "+id);
	}
	else // private msg
	{
		emit sendToServer("tell "+id+" hello!");
	}

	m_channels << id;
	m_addChannel->clear();
	
	return ctemp;
}

void KHChatWindow::enterToChannel(QListViewItem *item)
{
	if ( ! item )
	{
		return;
	}
	
	if ( m_chats[item->text(0)] )
	{
		m_tabWidget->showPage( m_chats[item->text(0)]->parentWidget());
		return;
	}
	
	QVBox *vbox = createPage(i18n("Channel #%1").arg(item->text(0)),item->text(0));
	vbox->setName(item->text(0)); // for delete later
	KHChatWindowPrivate *tmpChat = new KHChatWindowPrivate(vbox, item->text(0));
	connect(tmpChat, SIGNAL(sendMessageToServer(const QString &, const QString &)), SIGNAL(sendMessageToServer(const QString &, const QString &)));
	
	connect(tmpChat, SIGNAL(openChat(const QString &)), this, SLOT(chatWith(const QString&)));
	
	connect(tmpChat, SIGNAL(showed(const QString &)), this, SLOT(restore(const QString &)));
	
	m_chats.insert(item->text(0),tmpChat );
	
	requestNicks( item->text(0) );
}

void KHChatWindow::slotCancel()
{
}

void KHChatWindow::restore(const QString &id)
{
	KHChatWindowPrivate *tmpChat = m_chats[id];
	if ( tmpChat )
	{
		m_tabWidget->setTabColor(tmpChat->parentWidget(), Qt::black);
	}
}

void KHChatWindow::closeCurrent()
{
	if ( m_tabWidget->currentPage() != m_mainPage )
	{
		QWidget *wid = m_tabWidget->currentPage();
		m_tabWidget->removePage( wid );
		
		m_chats.remove( wid->name() );
		delete wid;
	}
}

void KHChatWindow::setText(const QString &from, const QString &toChannel, const QString &msg)
{
	kdDebug() << "TOCHANNEL: " << toChannel << endl;
	KHChatWindowPrivate *tmpChat = m_chats[toChannel];
	if ( tmpChat )
	{
		tmpChat->setNewMessage("<"+from+"> "+ msg+"\n");
		if ( ! tmpChat->isVisible() )
		{
			m_tabWidget->setTabColor(tmpChat->parentWidget(), Qt::green);
		}
	}
	else
	{
		if ( m_channels.find(toChannel) == m_channels.end() )
		{
 			kdDebug() << "Channel " << toChannel << " not exists... adding" << endl;
			addChannel(toChannel);
		}
	}
}

void KHChatWindow::setPrivateMessage(const QString &from, const QString &msg)
{
	KHChatWindowPrivate *tmpChat = m_chats[from];
	if ( tmpChat )
	{
		tmpChat->setNewMessage("<"+from+"> "+ msg+"\n");
		if ( ! tmpChat->isVisible() )
		{
			m_tabWidget->setTabColor(tmpChat, Qt::green);
		}
	}
	else
	{
		if ( m_channels.find(from) == m_channels.end() )
		{
			kdDebug() << "Private " << from << " not exists... adding" << endl;
			enterToChannel( addChannel(from) );
			setPrivateMessage(from, msg);
		}
	}
}

void KHChatWindow::keyPressEvent( QKeyEvent *e )
{
// 	e->ignore();
}

void KHChatWindow::requestNicks(const QString &id)
{
	bool ok = false;
	
	int channelId = id.toInt(&ok);
	
	if ( ok )
	{
		sendToServer(QString("in %1").arg(channelId));
	}
}

QVBox *KHChatWindow::createPage(const QString &caption, const QString &title)
{
	QVBox *box = new QVBox(m_tabWidget, title);
	
	new QLabel("<h4>"+caption+"</h4>", box);
	
	new KSeparator(box);
	
	m_tabWidget->addTab(box, title);
	
	return box;
}

void KHChatWindow::putChannelNicks(const QString &id, const QStringList &nicks)
{
	KHChatWindowPrivate *chat = m_chats[id];
	if ( chat )
	{
		chat->putNicks(nicks);
	}
}


// PRIVATE

KHChatWindowPrivate::KHChatWindowPrivate(QWidget *parent, const QString &channel ) : QHBox(parent), m_channel(channel),m_nicks(0)
{
	bool ok = false;
	
	m_channel.toInt(&ok);
	if ( ok )
	{
		m_nicks = new KHNickList(this);
// 		m_nicks->addColumn(SmallIcon("edu_languages"), i18n("Nicks"));
		
		m_menu = new KPopupMenu(this);
		m_menu->insertTitle( SmallIcon("user"), i18n("Chat"));
		m_menu->insertItem(i18n("Open chat"),0);
		m_menu->insertItem(i18n("Query user..."),1 );
		connect(m_menu, SIGNAL(activated(int)), this, SLOT(menuActivated(int)));
	
		connect(m_nicks, SIGNAL(contextMenu (QListBoxItem *, const QPoint &)), this, SLOT(showMenu(QListBoxItem *, const QPoint &)));
	}
	
	QVBox *m_box = new QVBox(this);
	m_textArea = new KHTextBox(m_box);

	m_lineEdit = new KLineEdit(m_box);
	connect(m_lineEdit, SIGNAL(returnPressed() ), this, SLOT(sendMessage()));
	show();
	
	if ( m_nicks )
	{
		m_nicks->setMaximumWidth(m_nicks->width()+2);
	}
}

KHChatWindowPrivate::~KHChatWindowPrivate() {}

void KHChatWindowPrivate::showMenu(QListBoxItem *i, const QPoint &p)
{
	if (i)
	{
		m_menu->popup(p);
	}
}

void KHChatWindowPrivate::menuActivated(int opt)
{
	switch(opt)
	{
		case 0:
		{
			emit openChat(m_nicks->currentNick());
		}
		break;
		default:
		{
			KMessageBox::information(this, i18n("Not implemented yet"));
		}
		break;
	}
}

void KHChatWindowPrivate::setNewMessage(const QString &msg)
{
	QString mtmp(msg);
	mtmp.remove( QRegExp("\\n|\\r|\\b") );
	m_textArea->append(mtmp+'\n');
}

void KHChatWindowPrivate::sendMessage()
{
	QString text =  m_lineEdit->text();
	
	bool ok = false;
	m_channel.toInt(&ok);
	if ( !ok )
	{
		// private msg
		setNewMessage("<"+PLAYER->ficsLogin()+"> "+text);
	}
	else
	{
		emit sendMessageToServer(m_channel, text);
	}
	m_lineEdit->clear();
}

void KHChatWindowPrivate::putNicks(const QStringList &nicks)
{
	if ( ! m_nicks )
	{
		return;
	}
	
	m_nicks->setNicks(nicks);
	
// 	
// 	for(uint i = 0; i < nicks.count(); i++)
// 	{
// 		KListViewItem *ntemp = new KListViewItem(m_nicks);
// 		ntemp->setText(0, nicks[i]);
// 		m_nicks->insertItem(ntemp);
// 	}
}

void KHChatWindowPrivate::showEvent(QShowEvent *e)
{
	bool ok = false;
	
	m_channel.toInt(&ok);
	if ( ok )
	{
		emit requestNicks(m_channel);
	}
	
	emit showed(m_channel);
}

#include "khchatwindow.moc"
