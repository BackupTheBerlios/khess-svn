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

#include "khengineiointerface.h"
#include <klocale.h>
#include <kdebug.h>
#include <qtextstream.h>

KHEngineIOInterface::KHEngineIOInterface() : KHIOInterface(), m_engine(0)
{
}


KHEngineIOInterface::~KHEngineIOInterface()
{
	closeResource();
}

void KHEngineIOInterface::connectToResource(const QString &command, const QString &args)
{
	m_engine = new KProcess(this);
	*m_engine << command;
	
	if ( ! args.isNull() )
	{
		*m_engine << args;
	}
	
	connect(m_engine, SIGNAL(receivedStdout (KProcess *, char *, int)), this, SLOT(resourceReadyToRead(KProcess *, char *, int)));
	connect(m_engine, SIGNAL(receivedStderr (KProcess *, char *, int)), this, SLOT(resourceReadyToRead(KProcess *, char *, int)));
	
	if( !m_engine->start(KProcess::OwnGroup, KProcess::All) )
	{
		kdError() << "I can't start process: " << command << " with arguments: " << "[ " << args << " ]" << endl;
	}
}

void KHEngineIOInterface::closeResource()
{
	if ( m_engine )
	{
		sendToResource("quit");
		
		if ( m_engine->isRunning() )
		{
			if ( m_engine->kill() )
			{
			}
			else
			{
				m_engine->kill(9);
			}
		}
	}
}

void KHEngineIOInterface::resourceReadyToRead(KProcess *proc, char *buffer, int buflen)
{
	QString data = QString::fromLatin1(buffer, buflen);
	
	kdDebug() << "[-" << data << "-]" << endl;
	
	if ( data.length() > 3 )
	{
		emit dataReaded(data);
	}
}

void KHEngineIOInterface::sendToResource(const QString &data)
{
	kdDebug() << "****Write to engine: " << data << endl;
	
	QString tmp = data;
	
	if ( tmp[tmp.length()] != '\n' )
	{
		tmp+='\n';
	}
	m_engine->writeStdin( tmp.latin1(), tmp.length() );
}

#include "khengineiointerface.moc"
