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

#ifndef KHCHESSMOVE_H
#define KHCHESSMOVE_H

#include <position.h>

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHChessMove
{
	public:
		KHChessMove();
		virtual ~KHChessMove();
		
		void copyData(const Position& fromPosition, const Position &toPosition);
		
		virtual void reset();
		
		void setFromRank(int value);
		
		void setFromFile(int value);
		void setToRank(int value);
		void setToFile(int value);
		void setManTaken(int value);
		void setSan(const QString &SAN);
		void setCan(const QString &CAN);
		void setNAG(int nag);
		void setPromote(QChar p);
		
		int fromRank() const;
		int fromFile() const;
		int toRank() const;
		int toFile() const;
		QChar promote() const;
		
		QString san() const;
		QString can() const;
		
		int manTaken() const;
		
		void debug();
		
		QString gameId() const;
		void setGameId(const QString &id);
		
	private:
		QString m_gameId; // Identify the move's owner
		int m_fromFile;
		int m_fromRank;
		int m_toFile;
		int m_toRank;
		QChar m_promote;
		int m_manTaken;
		QString m_SAN;
		QString m_CAN;
		int m_NAG;
};

#endif
