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

#include "khsimpleclock.h"
#include <qpalette.h>
#include <qapplication.h>

KHSimpleClock::KHSimpleClock(QWidget *parent, const char *name)
	: QHBox(parent, name)
{
	setup();
}

KHSimpleClock::KHSimpleClock( int hours, int minutes, int seconds, QWidget *parent, const char *name) : QHBox(parent, name)
{
	setup();
	
	setTime(hours, minutes, seconds );
}

void KHSimpleClock::setup()
{
	setFrameStyle( QFrame::Box | QFrame::Raised  );
	setMidLineWidth(4);
	setLineWidth(4);
	
	m_hours = new QLCDNumber ( 2, this,"hours");
	m_hours->setSegmentStyle( QLCDNumber::Filled );
	
	applyColorToLCD(m_hours);
	
	m_minutes = new QLCDNumber ( 2, this,"minutes");
	applyColorToLCD(m_minutes);
	
	m_seconds = new QLCDNumber (2, this, "seconds");
	applyColorToLCD(m_seconds);
}

void KHSimpleClock::applyColorToLCD(QLCDNumber *lcd)
{
	QColorGroup group = QApplication::palette().active();
	const QColor bg( 0x2e517f );
	const QColor fg( Qt::green );
	
	group.setColor( QColorGroup::Foreground, fg );
	group.setColor( QColorGroup::Background, bg );

	QPalette pal(group, group, group);
	lcd->setPalette(pal);
	lcd->setSegmentStyle( QLCDNumber::Flat  );
	lcd->resize(20,20);
}


KHSimpleClock::~KHSimpleClock()
{
	
}

void KHSimpleClock::advance()
{
	m_seconds->display(m_seconds->value() - 1);
	
	if(m_seconds->value() == 0 && m_minutes->value() == 0 && m_hours->value() == 0 )
	{
		emit(timerOut());
		return;
	}
	else
	{
		if(m_seconds->value() < 0 )
		{
			m_seconds->display(60 - 1);
			m_minutes->display(m_minutes->value() - 1);
			if(m_minutes->value() < 0 )
			{
				m_minutes->display(60 - 1);
				m_hours->display(m_hours->value() - 1);
			}
		}
	}
}

bool KHSimpleClock::setHours(int val)
{
	if(val >= 0 && val < 60)
	{
		m_hours->display(val);
		return true;
	}

	return false;

}

bool KHSimpleClock::setMinutes(int val)
{
	if (val < 0 )
	{
		return false;
	}
	
	int tmp = 0;
	
	if(val > 60)
	{
		setHours ( val / 60 );
		m_minutes->display ( val % 60 );
	}
	else
	{
		m_minutes->display(val);
	}
	return true;
}

bool KHSimpleClock::setSeconds(int val)
{
	if (val < 0 )
	{
		return false;
	}
	
	int tmp = 0;
	
	if(val > 60)
	{
		setMinutes ( val / 60 );
		m_seconds->display ( val % 60 );
	}
	else
	{
		m_seconds->display(val);
	}
	return true;
}


bool KHSimpleClock::setTime(int hours, int minutes, int seconds )
{
	return ( setHours( hours ) && setMinutes( minutes ) && setSeconds( seconds));
}

void KHSimpleClock::setIncrement(int value)
{
	if ( value < 59 )
	{
		m_seconds->display( m_seconds->value() + value );
	}
}



#include "khsimpleclock.moc"
