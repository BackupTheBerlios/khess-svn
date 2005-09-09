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
#ifndef KHINTERNETWIDGET_H
#define KHINTERNETWIDGET_H

#include <kmdichildview.h>
#include <qvbox.h>
#include <ktabwidget.h>

#include "khconsole.h"
#include "khseektab.h"
#include "khmatch.h"
#include "khficsmove.h"

#include "khparserbase.h"

class KHInternetIOInterface;
class KHChatWindow;
class KHFicsParser;

/**
 * @author David Cuadrado <krawek@gmail.com >
*/
class KHInternetWidget : public QVBox
{
	Q_OBJECT
	public:
		KHInternetWidget(QWidget *parent = 0);
		~KHInternetWidget();
		
	public slots:
		void connectToServer();
		void closeConnection();
		void showChatWindow();
		void showShout(const QString &, const QString &);
		void sendPage(const QString &url, const QString &title);
		void sendAMove(KHFicsMove &);
		
	private:
		void setupTabs();
		
	signals:
		void showWebPage(KMdiChildView * );
		void message2osd(const QString &);
		void createNewMatch(KHMatch *);
		void sendFicsMove(KHChessMove *);
		void sendMoveToServer(const KHChessMove &);
		
	private:
		KTabWidget *m_tabBar;
		KHConsole *m_console;
		KHSeekTab *m_seekTab;
		
		KHChatWindow *m_chat;
		KHFicsParser *m_parser;

};

#endif
