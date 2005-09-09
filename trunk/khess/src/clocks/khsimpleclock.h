/***************************************************************************
 *   Copyright (C) 2005 by jorge,,,   *
 *   kuadrosx@libano   *
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
#ifndef KHSIMPLECLOCK_H
#define KHSIMPLECLOCK_H

#include <qhbox.h>
#include <qlcdnumber.h>

/**
@author jorge,,,
*/
class KHSimpleClock : public QHBox
{
	Q_OBJECT
	public:
		KHSimpleClock(QWidget *parent = 0, const char *name = 0);
		KHSimpleClock( int hours, int minutes, int seconds, QWidget *parent, const char *name = 0);
		~KHSimpleClock();
		
	private:
		void setup();
		void applyColorToLCD(QLCDNumber *lcd);
		
	private:
		QLCDNumber *m_hours, *m_minutes, *m_seconds;

	public slots:
		virtual void advance();
		bool setHours(int val);
		bool setMinutes(int val);
		bool setSeconds(int val);
		bool setTime(int hours, int minutes, int seconds);
		void setIncrement(int value);
		
	signals:
		void timerOut();

};

#endif
