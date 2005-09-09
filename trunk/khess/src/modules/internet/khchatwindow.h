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
#ifndef KHCHATWINDOW_H
#define KHCHATWINDOW_H

#include <kdialog.h>
#include <ktabwidget.h>
#include <klistview.h>
#include <klineedit.h>
#include <ktextedit.h>
#include <kpopupmenu.h>

#include <qvbox.h>
#include <qmap.h>

#include "khnicklist.h"


#include "khtextbox.h"

class KHChatWindowPrivate;

typedef QMap<QString, KHChatWindowPrivate*> ChannelHash;

/**
 * @author David Cuadrado <krawek@gmail.com >
*/
class KHChatWindow : public KDialog
{
	Q_OBJECT
	public:
		KHChatWindow(QWidget *parent = 0);
		~KHChatWindow();
		
		QVBox *createPage(const QString &caption, const QString &title);
		
	public slots:
		void setText(const QString &from, const QString &toChannel, const QString &msg);
		QListViewItem * addChannel(const QString &id, const QString &name = "");
		void setPrivateMessage(const QString &from, const QString &msg);
		void closeCurrent();
		void putChannelNicks(const QString &, const QStringList &);
		void requestNicks(const QString &id);
		void pushChannels(const QValueList<QStringList> &);
		void chatWith(const QString &nick);
		
	private slots:
		void enterToChannel(QListViewItem *);
		void slotCancel();
		void showMenu(KListView *l, QListViewItem *i, const QPoint &p);
		void menuActivated(int);
		void restore(const QString &);
		
	signals:
		void sendMessageToServer(const QString &m_channel, const QString &msg);
		void sendToServer(const QString &command);
		
		
	protected:
		void keyPressEvent( QKeyEvent *e );
		
	private:
		KListView *m_listView;
		KLineEdit *m_addChannel;
		ChannelHash m_chats;
		QStringList m_channels;
		KTabWidget *m_tabWidget;
		QVBox *m_mainPage;
		KPopupMenu *m_menu;
};

class KHChatWindowPrivate : public QHBox
{
	Q_OBJECT 
	public:
		KHChatWindowPrivate(QWidget *parent , const QString &channel);
		~KHChatWindowPrivate();
		
	public slots:
		void setNewMessage(const QString &msg);
		void putNicks(const QStringList &nicks);
		
	private slots:
		void sendMessage();
		void showMenu(QListBoxItem *, const QPoint &);
		void menuActivated(int);
		
	signals:
		void sendMessageToServer(const QString &m_channel, const QString &msg);
		void requestNicks(const QString &id);
		void openChat(const QString &);
		void showed(const QString &id);
		
	private:
		KHTextBox *m_textArea;
		KLineEdit *m_lineEdit;
		QString m_channel;
		KHNickList *m_nicks;
		KPopupMenu *m_menu;
		
	protected:
		virtual void showEvent(QShowEvent *e);
};

#endif
