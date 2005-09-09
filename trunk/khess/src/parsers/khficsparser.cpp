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

#include "khficsparser.h"

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>

#include <qregexp.h>

KHFicsParser::KHFicsParser(const ConnectionParam& cp) : KHParserBase(), m_isLogged(false), m_userParam(cp)
{
	m_interface = new KHInternetIOInterface ;
	
	connect(static_cast<KHInternetIOInterface*>(m_interface), SIGNAL(dataReaded(const QString &)), this, SLOT(parseData(const QString &)));
	
	m_interface->connectToResource( cp.server(), QString::number(cp.port()) );
	
}


KHFicsParser::~KHFicsParser()
{
	kdDebug() << "[Destroying KHFicsParser]" << endl;
}

#include <iostream>

void KHFicsParser::parseData(const QString &data)
{
	std::cout << "****** PARSING DATA ********" << std::endl;
	
	QRegExp start(".* Starting FICS session as (\\w*).*");
	
	QRegExp chatMsg("(^.*)+\\(([\\d]{1,2})\\): (.*)");
	QRegExp privateMsg("(^.*) tells you: (.*)");
	QRegExp shoutMsg("(^.*) shouts: (.*)");
	QRegExp newGame("^[(.* accepts your seek.)(Your seek matches one \\w posted by .*\\.)]*(Creating|Continuing): .*\\{Game \\d+.*(<12>.*)");
	QRegExp endGame("\\{?Game (\\d+) \\(.*\\) .*(resigns|drawn|adjourned|aborted|checkmated).* ([01])-([01]).*");
	QRegExp seek("^(.*) \\(.*\\) seeking ([\\d]+) ([\\d]+) ([\\w]+) ([\\w]+).*");
	QRegExp style12("^\\s*<12>.*");
	QRegExp sought("(\\d+)\\s+([(\\d+)(\\+\\+\\+\\+)(\\-\\-\\-\\-)]*)\\s+([\\w+\\(\\w\\)]*)\\s+(\\d+)\\s+(\\d+)\\s+(rated|unrated)\\s+(standard|blitz)");
	QRegExp channels("(\\d+)\\s+(\\D+)");
	
	QRegExp nicks("Channel (\\d+) \\\"\\w+\\\": ");
	
	QString buffer = data;
	
// 	buffer.remove(QRegExp("\\n\\s*(\\\\)"));
	buffer.remove(QRegExp("\\n\\r"));
	
// 	buffer.replace('\n', " ");
	
	std::cout << "BUFFER: "<< buffer << std::endl;
	
	emit sendToConsole(data);
	
	if ( data.length() < 3 ) //garbage
	{
		return;
	}
	
	
	if ( buffer.startsWith("<12>") )
	{
		// Style 12 is the default style for fics interfaces
		
		QStringList lines = QStringList::split('\n', data);
		
		parse12style(lines[0], false);
		
		if ( lines.count() > 1 )
		{
			if (endGame.search(lines[1]) > -1 )
			{
				// 1: result string; 2: white result; 3: black result
				QStringList list = endGame.capturedTexts();

				parseResult(list[2], list[3], list[4]);
			}
		}
	}
	if ( seek.search(buffer) > -1 )
	{
		std::cout << "seek" << std::endl;
		QStringList list = seek.capturedTexts();
		std::cout << list[1] << " buscando patida " << std::endl;
	}
	else if ( chatMsg.search(buffer) > -1 )
	{
		QStringList list = chatMsg.capturedTexts();
		
		if ( list[1] == PLAYER->ficsLogin() )
		{
			list[3].remove(QRegExp("(\\(told \\d+ players in channel.*\\))"));
		}
		
		emit sendToChat(list[1], list[2], list[3]);
	}
	else if ( privateMsg.search(buffer) > -1 )
	{
		// pos: 1 & 2
		QStringList list = privateMsg.capturedTexts();
		
		emit sendPrivateMessage(list[1], list[2]);
		
	}
	else if ( shoutMsg.search(buffer) > -1 )
	{
		// pos: 1 & 2
		kdDebug() << "*********SHOUT*********" << endl;
		QStringList list = shoutMsg.capturedTexts();
		emit sendShout(list[1], list[2]);
	}
	else if ( newGame.search(buffer) > -1 )
	{
		QStringList list = newGame.capturedTexts();
		
		for( uint i = 0; i < list.count(); i++)
		{
			std::cout << "ARG: " << list[i] << std::endl;
		}
		kdDebug() << "NEW GAME " << endl;
// 		std::cout << "NEW GAME ID: " << list[2] << std::endl;
		parse12style(list[2], true);
	}
	else if (! m_isLogged )
	{
		doLoginProcess(buffer);
	}
	else if ( start.search(buffer) > -1 )
	{
		PLAYER->setFicsLogin(start.capturedTexts()[1]);
		sendUserOptions();
	}
	else if ( nicks.search(buffer) > -1)
	{
		QString cid = nicks.cap(1);
		buffer.remove('\\');
		buffer.remove(nicks);
		buffer.remove(QRegExp("\\d+ players are in channel \\d+\\s\\.") );
		
		QStringList channelNicks = QStringList::split(' ', buffer);
		emit sendChannelNicks(cid, channelNicks);
	}
	else if ( sought.search(buffer) > -1)
	{
		QValueList<QStringList> seeks;
		/*
		 * 0: id
		 * 1: elo
		 * 2: login
		 * 3: time
		 * 4: increment
		 * 5: is rated
		 * 6: variant
		*/
		while ( true )
		{
			if ( sought.search(buffer) == -1 )
			{
				kdDebug() << "END SOUGHT PARSING" << endl;
				break;
			}
	
			QStringList list = sought.capturedTexts();
			
			list.pop_front();
			
			seeks << list;
			
			for (uint i = 0; i < list.count(); i++)
			{
				kdDebug() << i << " " << list[i].remove('\n') << endl;
			}
		
			buffer.remove(list[0]);
		}
		
		emit sendSeeks(seeks);
	}
	else if ( buffer.contains("Type [next] to see next page."))
	{
		execCommand("next");
	}
// 	else if ( channels.search(buffer) > -1)
// 	{
// 		QValueList<QStringList> channelsList;
// 		
// 		while ( true )
// 		{
// 			if ( channels.search(buffer) == -1 )
// 			{
// 				kdDebug() << "END CHANNEL PARSING" << endl;
// 				break;
// 			}
// 	
// 			QStringList list = channels.capturedTexts();
// 			
// 			list.pop_front();
// 			
// 			channelsList << list;
// 			
// // 			for (uint i = 0; i < list.count(); i++)
// // 			{
// // 				kdDebug() << i << " " << list[i].remove('\n') << endl;
// // 			}
// 		
// 			buffer.remove(list[0]);
// 		}
// 		
// 		emit sendChannels(channelsList);
// 	}
	
	kdDebug() << "****** END PARSING DATA ********" << endl;
}

void KHFicsParser::parse12style(const QString &data, bool begin)
{
	kdDebug() << "Parse 12 style :" << data << endl;
	QStringList fields = QStringList::split( QChar(' '), data, false );
	
	if ( fields.count() < 10 )
	{
		return;
	}
	
	if ( begin ) // Start the game!
	{
		// a new game that we are playing, or observing
		
		KHWarrior *white, *black;

		kdDebug() << "PLAYER: " << m_userParam.login() << endl;
		if((PLAYER->ficsLogin().upper() == fields[17].upper()) && ((fields[19].toInt() == -1) || (fields[19].toInt() == 1)))
		{
			white = PLAYER;
			black = new KHWarrior();
			black->setWarriorInformation( SmallIcon("network"), fields[18], fields[18], "Internet");
		}
		else if((PLAYER->ficsLogin().upper() == fields[18].upper()) && ((fields[19].toInt() == -1) || (fields[19].toInt() == 1)))
		{
			white = new KHWarrior();
			white->setWarriorInformation( SmallIcon("network"), fields[17], fields[17], "Internet");
			black = PLAYER;
		}
		else
		{
			kdDebug() << "Game mode not supported! "<< fields[19] << endl;
			return;
		}

 		white->setElo(1700);
 		black->setElo(1700); // TODO
		
		KHMatch *m_match = new KHMatch(KHMatch::INTERNET, white, black, "fics:"+fields[16], false);
		
		m_match->setTime( fields[20].toInt() * 60, fields[21].toInt());

		emit createMatch(m_match);
	}
	else // A move!
	{
		KHFicsMove move;
		move.reset();
		QString board = "",tmp = "";
		
// 		// various ICS stuff for ChessMove
		move.setIcsClockTicking (fields[31].toInt());
		move.setIcsPawnPushFile( fields[10].toShort());
		move.setIcsMoveCounter( fields[15].toInt());
		
		if(fields[9] == "W")
		{
			move.setIcsOnMove(false);
		}
		else
		{
			move.setIcsOnMove(true);
		}

// 		switch( fields[19].toInt() )
// 		{
// 			// Examining a game
// 			case 2:
// 				move.ICS_Mode = ICS_Examine;
// 				if(myParent->isActiveGame(fields[16].toInt()))
// 				{
// 					// no new game started yet, call parsestyle 12 with a different mode
// 					parseStyle12(line, PARSE12_MODE_NEW);
// 					return;
// 				}
// 				break;
// 			// Observing a game
// 			case -2:
// 			case 0:
// 				move.ICS_Mode = ICS_Observe;
// 				break;
// 			// Playing a game
// 			default:
// 				move.ICS_Mode = ICS_Normal;
// 				break;
// 		}
// 
// 		// Verbose Coordinate Notation of previous move ( USE AS CAN )
		tmp = fields[27].right( fields[27].length() - 2 ).replace( "-", "" );
		move.setCan(tmp);
// 
// 		// SAN
		tmp = fields[29].replace( "+", "" ).replace( "#", "" );
		move.setSan(tmp);
		
		// fill the board
		for(uint i = 1; i < 9; i++)
		{
			board += fields[i];
		}
		
		board += fields[11]+fields[12]+fields[13]+fields[14];
		
		move.setBoard(board);
		move.setGameId("fics:"+fields[16]);
		move.setTimes(fields[24].toInt(), fields[25].toInt());
		
		move.debug();
		
		emit doFicsMove( move );
	}
}

void KHFicsParser::parseResult( const QString &str, const QString &wr, const QString &br)
{
	QString result;
	result = wr+"-"+br;
	
	if (str == "resigns")
	{
	}
	else if ( str == "drawn" )
	{
		result = "1-1";
	}
	else if ( str == "adjourned")
	{
	}
	else if ( str == "aborted")
	{
		result= "0-0";
	}
	else if ( str == "checkmated")
	{
	}
	
	kdDebug() << "Finalizando el juego con : " << str << " " << result << endl;
	
	emit endGame("Game finished", result);
}

void KHFicsParser::doLoginProcess(const QString &data) // FIXME: send true data
{
	kdDebug() << "AQUI CON DATA: " << data << endl;
	if (data.contains("login:") )
	{
		static_cast<KHInternetIOInterface*>(m_interface)->sendToResource( m_userParam.login() );
		kdDebug() << "ENVIANDO LOGIN: " << m_userParam.login() << endl;
	}
	else if ( data.contains("Logging you in as") || data.contains( "is not a registered name" ) && m_userParam.guest() )
	{
		m_isLogged = true;
		static_cast<KHInternetIOInterface*>(m_interface)->sendToResource( " " );
	}
	else if ( data.contains("password:") )
	{
		static_cast<KHInternetIOInterface*>(m_interface)->sendToResource( m_userParam.password() );
		m_isLogged = true;
	}
	
	// TODO: handler for confirmation, send true params
}

void KHFicsParser::sendUserOptions()
{	
	static_cast<KHInternetIOInterface*>(m_interface)->sendToResource("set style 12");
	static_cast<KHInternetIOInterface*>(m_interface)->sendToResource("set interface Khess 0.1alpha");
	
	QTimer::singleShot(3000, this, SLOT(updateSeeks()));
}

bool KHFicsParser::haveConnection()
{
	return static_cast<KHInternetIOInterface*>(m_interface)->isConnected();
}

void KHFicsParser::sendChatMessage(const QString &channel, const QString &msg)
{
	
	execCommand("tell "+channel+" "+msg);
}

void KHFicsParser::sendMove(const KHChessMove &move)
{
	execCommand( move.san() );
}

void KHFicsParser::updateSeeks()
{
// 	execCommand( "sought all" );
// 	QTimer::singleShot(20000, this, SLOT(updateSeeks()));
}

#include "khficsparser.moc"
