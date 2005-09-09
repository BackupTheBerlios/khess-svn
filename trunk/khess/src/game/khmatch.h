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

#ifndef KHMATCH_H
#define KHMATCH_H

#include <qobject.h>
#include "khclock.h"

class KHWarrior;

/**
 * Class for manage matchs information
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHMatch : public QObject
{
	Q_OBJECT
	public:
		enum Type
		{
			NONE = 0,
			LOCAL,
			ENGINE,
			INTERNET
		};
		KHMatch(Type t, KHWarrior *white, KHWarrior *black, const QString &id, bool rated = false);
		~KHMatch();
		void setTime(int base, int inc);
		KHWarrior *black();
		KHWarrior *white();
		QString id() const;
		KHClock *clock();
		int type() const;
		int time();
		int increment();
		
	private:
		KHWarrior *m_white;
		KHWarrior *m_black;
		
		bool m_rated;
		int m_time, m_increment;
		QString m_id;
		KHClock *m_clock;
		Type m_type;
};

#endif
