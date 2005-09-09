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

#include "khmatch.h"
#include <kdebug.h>
#include <klocale.h>

#include "khwarrior.h"

KHMatch::KHMatch(Type type, KHWarrior *white, KHWarrior *black, const QString &id, bool rated) : QObject(0, "KHMatch"), m_white(white), m_black(black), m_rated(rated), m_id(id), m_type(type)
{
	kdDebug() << "New match : " << white->theName() << " vs. " << black->theName() << endl;
	m_clock = new KHClock(this);
}


KHMatch::~KHMatch()
{
	kdDebug() << "[Destroying KHMatch]" << endl;
}

KHWarrior *KHMatch::black()
{
	return m_black;
}

KHWarrior *KHMatch::white()
{
	return m_white;
}

void KHMatch::setTime(int base, int inc)
{
	m_time = base;
	m_increment = inc;
	
	m_clock->setInitTime(base, inc);
}

QString KHMatch::id() const
{
	return m_id;
}

KHClock *KHMatch::clock()
{
	return m_clock;
}

int KHMatch::type() const
{
	return m_type;
}

int KHMatch::time()
{
	return m_time;
}

int KHMatch::increment()
{
	return m_increment;
}


#include "khmatch.moc"
