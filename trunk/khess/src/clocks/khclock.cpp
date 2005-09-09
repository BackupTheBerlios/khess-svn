/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
 
#include "khclock.h"
#include <kdebug.h>

KHClock::KHClock(QObject *parent, const char *name) : QObject(parent, name), m_turn(true), m_increment(0)
{
	m_displayLight = new KHSimpleClock();
	m_cronLight = new QTimer(this);
	connect( m_cronLight, SIGNAL(timeout()), m_displayLight, SLOT(advance()));
	
	m_displayBlack = new KHSimpleClock();
	m_cronBlack = new QTimer(this);
	connect( m_cronBlack, SIGNAL(timeout()), m_displayBlack, SLOT(advance()));

}

KHClock::~KHClock()
{
}

void KHClock::setInitTime(int base, int inc)
{
	kdDebug() << "Setting init time to " << base << " " << inc << endl;
	m_displayLight->setSeconds(base);
	m_displayBlack->setSeconds(base);
	m_increment = inc;
}

void KHClock::setNewTime(int whiteSeconds, int blackSeconds, bool bandOnTurn)
{
	kdDebug() << "SET TIMES: " << whiteSeconds << " " << blackSeconds << endl;
	m_displayLight->setSeconds(whiteSeconds);
	m_displayBlack->setSeconds(blackSeconds);
	m_turn = bandOnTurn;
	play();
}

void KHClock::stop()
{
	m_cronLight->stop();
	m_cronBlack->stop();
}

void KHClock::resume()
{
	play();
}

void KHClock::play()
{
	if(m_turn)
	{
		m_cronLight->stop();
		m_displayBlack->setIncrement(m_increment);
		m_cronBlack->start (1000); 
	}
	else
	{
		m_cronBlack->stop();
		m_displayLight->setIncrement(m_increment);
		m_cronLight->start(1000); 
	}
	m_turn = !m_turn;
	emit playerChanged();
}

KHSimpleClock *KHClock::displayBlack()
{
	return m_displayBlack;
}
	
KHSimpleClock *KHClock::displayLight()
{
	return m_displayLight;
}

#include "khclock.moc"
