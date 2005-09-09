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
#ifndef KHCLOCK_H
#define KHCLOCK_H

#include <qwidget.h>
#include <qtimer.h>
#include "khsimpleclock.h"


/**
 * @author Jorge Cuadrado
 * @author David Cuadrado
*/

class KHClock : public QObject
{
	Q_OBJECT
	public:
		KHClock(QObject *parent = 0, const char *name = 0);
		~KHClock();
		KHSimpleClock *displayBlack();
		KHSimpleClock *displayLight();
		void setInitTime(int base, int inc);
		void setNewTime(int whiteSeconds, int blackSeconds, bool bandOnTurn);
		
	private:
		QTimer *m_cronBlack, *m_cronLight;
		KHSimpleClock *m_displayBlack, *m_displayLight;
		bool m_turn;
		int m_increment;
		
	public slots:
		void stop();
		void play();
		void resume();
		
	signals:
		void playerChanged();
};

#endif
