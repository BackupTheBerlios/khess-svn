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

#include "khchessmove.h"

KHChessMove::KHChessMove()
{
	reset();
}


KHChessMove::~KHChessMove()
{
}

void KHChessMove::reset()
{
	kdDebug() << "RESET MOVE" << endl;
	m_fromFile = -1;
	m_fromRank = -1;
	m_toFile = -1;
	m_toRank = -1;
	m_promote = -1;
	m_manTaken = -1;
	m_NAG = 0;
	m_promote = ' ';
}

void KHChessMove::setGameId(const QString &id)
{
	m_gameId = id;
}

QString KHChessMove::gameId() const
{
	return m_gameId;
}

void KHChessMove::debug()
{
	kdDebug() << "******DEBUGGING CHESS MOVE******" << endl;
	
	kdDebug() << "fromFile: " << m_fromFile << " toFile " << m_toFile << endl;
	kdDebug() << "fromRank: " << m_fromRank << " toRank " << m_toRank << endl;
	
	kdDebug() << "SAN: " << m_SAN << endl;
	kdDebug() << "CAN: " << m_SAN << endl;
	
	kdDebug() << "********************************" << endl;
}

void KHChessMove::copyData(const Position& fromPosition, const Position &toPosition)
{
	m_fromRank = fromPosition.rank();
	m_fromFile = fromPosition.file();
	
	m_toFile = toPosition.file();;
	m_toRank = toPosition.rank();
}

void KHChessMove::setFromRank(int value)
{
	m_fromRank = value;
}

int KHChessMove::fromRank() const
{
	return m_fromRank;
}

int KHChessMove::fromFile() const
{
	return m_fromFile;
}
int KHChessMove::toRank() const
{
	return m_toRank;
}
int KHChessMove::toFile() const
{
	return m_toFile;
}

void KHChessMove::setFromFile(int value)
{
	m_fromFile = value;
}
void KHChessMove::setToRank(int value)
{
	m_toRank = value;
}
void KHChessMove::setToFile(int value)
{
	m_toFile = value;
}

void KHChessMove::setManTaken(int value)
{
	m_manTaken = value;
}

void KHChessMove::setSan(const QString &SAN)
{
	m_SAN = SAN;
}

void KHChessMove::setCan(const QString &CAN)
{
	m_CAN = CAN;
}

void KHChessMove::setNAG(int nag)
{
	m_NAG = nag;
}


QString KHChessMove::san() const
{
	return m_SAN;
}

QString KHChessMove::can() const
{
	return m_CAN;
}

int KHChessMove::manTaken() const
{
	return m_manTaken;
}

void KHChessMove::setPromote(QChar p)
{
	m_promote = p;
}

QChar KHChessMove::promote() const
{
	return m_promote;
}
