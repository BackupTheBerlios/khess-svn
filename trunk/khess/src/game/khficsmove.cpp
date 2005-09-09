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

#include "khficsmove.h"

KHFicsMove::KHFicsMove() : KHChessMove()
{
}


KHFicsMove::~KHFicsMove()
{
}

void KHFicsMove::reset()
{
	m_icsMoveCounter = 0;
	m_icsPawnPushFile = -1;
	m_icsMode = -1;
	m_icsOnMove = 0;
	m_icsClockTicking = true;
	m_board = "";
	
	KHChessMove::reset();
}


void KHFicsMove::setTimes(int whitetime, int blacktime)
{
	m_whiteTime = whitetime;
	m_blackTime = blacktime;
}

void KHFicsMove::setBoard(const QString &board)
{
	m_board = board;
}

QString KHFicsMove::board()
{
	return m_board;
}


// FOR ICS!!

int KHFicsMove::icsMode()
{
	return m_icsMode;
}

int KHFicsMove::icsPawnPushFile()
{
	return m_icsPawnPushFile;
}

int  KHFicsMove::icsMoveCounter()
{
	return m_icsMoveCounter;
}

bool KHFicsMove::icsOnMove()
{
	return m_icsOnMove;
}

bool KHFicsMove::icsClockTicking()
{
	return m_icsClockTicking;
}

void KHFicsMove::setIcsMode(int mode)
{
	m_icsMode = mode;
}

void KHFicsMove::setIcsPawnPushFile(int ppf)
{
	m_icsPawnPushFile = ppf;
}

void KHFicsMove::setIcsMoveCounter(int mc)
{
	m_icsMoveCounter = mc;
}

void KHFicsMove::setIcsOnMove(bool who)
{
	m_icsOnMove = who;
}

void KHFicsMove::setIcsClockTicking(bool ct)
{
	m_icsClockTicking = ct;
}

int KHFicsMove::blackTime()
{
	return m_blackTime;
}

int KHFicsMove::whiteTime()
{
	return m_whiteTime;
}

