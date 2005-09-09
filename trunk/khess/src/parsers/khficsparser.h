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

#ifndef KHFICSPARSER_H
#define KHFICSPARSER_H

#include <khparserbase.h>
#include <qtimer.h>

#include "khinternetiointerface.h"
#include "khmatch.h"
#include "khwarrior.h"
#include "khficsmove.h"

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHFicsParser : public KHParserBase
{
	Q_OBJECT

	public:
		KHFicsParser(const ConnectionParam&);
		~KHFicsParser();
		void doLoginProcess(const QString &data);
		bool haveConnection();
		void sendUserOptions();
		void parse12style(const QString &data, bool begin = true);
		void parseResult( const QString &str, const QString &wr, const QString &br);
		
	public slots:
		void sendMove(const KHChessMove &);
		void updateSeeks();
		
	protected slots:
		void parseData(const QString &data);
		void sendChatMessage(const QString &, const QString &);
		
	signals:
		void sendToChat(const QString &from, const QString &toChannel, const QString &msg);
		void sendPrivateMessage(const QString &from, const QString &msg);
		void sendShout(const QString &from, const QString &msg);
		void sendToConsole(const QString &data);
		
		void createMatch(KHMatch *);
		void doFicsMove(KHFicsMove &);
		void endGame(const QString &msg, const QString &result);
		void sendChannelNicks(const QString &id, const QStringList &nicks);
		void sendSeeks(const QValueList<QStringList> &);
		void sendChannels(const QValueList<QStringList> &);
		
	private:
		bool m_isLogged;
		ConnectionParam m_userParam;

};

#endif
