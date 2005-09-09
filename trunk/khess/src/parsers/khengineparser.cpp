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

#include "khengineparser.h"
#include <klocale.h>
#include <kdebug.h>
#include <qregexp.h>

KHEngineParser::KHEngineParser(KHMatch *match) : KHParserBase(), m_init(false), m_match(match)
{
	QString command ="", args ="";
	m_interface = new KHEngineIOInterface ;
	
	connect(static_cast<KHEngineIOInterface*>(m_interface), SIGNAL(dataReaded(const QString &)), this, SLOT(parseData(const QString &)));
	
	bool whiteEngine = false, blackEngine = false;
	if ( match->white()->type() == KHWarrior::Engine )
	{
		whiteEngine = true;
		command = match->white()->setting("Command");
		args = match->white()->setting("Arguments");
	}

	if (match->black()->type() == KHWarrior::Engine )
	{
		blackEngine = true;
		command = match->black()->setting("Command");
		args = match->black()->setting("Arguments");
	}
				
	if ( whiteEngine && blackEngine ) // 2 motores
	{
	}
	
	m_interface->connectToResource( command, args );
}


KHEngineParser::~KHEngineParser()
{
	kdDebug() << "[Destroying KHEngineParser]" << endl;
	m_interface->sendToResource("quit");
	m_interface->closeResource();
	delete m_interface;
}

void KHEngineParser::parseData(const QString &data)
{
	kdDebug() << "BUFFER: >>>" << data <<"<<<"<< endl;

	if ( ! m_init )
	{
		m_interface->sendToResource("xboard\nprotover 2");
		
		m_interface->sendToResource("hard");
		m_interface->sendToResource(QString("time %1/%2").arg( m_match->time()).arg(m_match->time()/60) ); // FIXME: time
		
// 		initialize();
		
		m_init = true;
	}
	
	QStringList fields = QStringList::split('\n', data);
	
	QStringList::iterator it;
	
	QRegExp move("^move (.*)");
	
	for( it = fields.begin();it != fields.end(); ++it )
	{
		if ( (*it).length() < 3 )
		{
			continue;
		}
		else
		if ( move.search(*it) > -1)
		{
			kdDebug() << "MOVE " << move.cap(1) << endl;
			KHChessMove mv;
			mv.setCan(move.cap(1));
			mv.setSan(move.cap(1));
			mv.setGameId(m_match->id());
			emit doMove(&mv);
		}
		else if ( (*it).contains("feature") && (*it).contains("done="))
		{
			initialize();
			m_interface->sendToResource(QString("sd %1").arg(m_match->white()->setting("Depth") ) );
			m_interface->sendToResource(QString("time %1/%2").arg( m_match->time()).arg(m_match->time()/60) );
		}
	}
}

void KHEngineParser::initialize()
{
// 	m_interface->sendToResource("log off");

// 	m_interface->sendToResource("random");
	
// 	m_interface->sendToResource("new");
	if (m_match->black()->type() == KHWarrior::Human )
	{
// 		m_interface->sendToResource(QString("sd %1").arg(m_match->white()->setting("Depth") ) );
		m_interface->sendToResource("go");
	}
}

void KHEngineParser::sendMoveToEngine(const KHChessMove &move)
{
	m_interface->sendToResource(move.can());
}


#include "khengineparser.moc"
