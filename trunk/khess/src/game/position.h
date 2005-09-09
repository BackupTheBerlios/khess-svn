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
#ifndef POSITION_H
#define POSITION_H

#include <qobject.h>

/**
	@author David Cuadrado <krawek@gmail.com >
*/
class Position
{
	public:
		enum NoteType
		{
			NOTE_NONE = 0,
			NOTE_HIGHLIGHT,
			NOTE_SELECT,
			NOTE_MOVE,
			NOTE_CASTLE,
			NOTE_PAWN_DOUBLE,
			NOTE_ATTACK,
			NOTE_ENPASSANT
		};
		
		Position();
		Position(int file, int rank, int manptr, NoteType note);
		~Position();
		void setFile(int file);
		void setRank(int rank);
		void setManPtr(int mnptr);
		void setNote (NoteType note);
		
		
		int file() const;
		int rank() const;
		int manPtr() const;
		int note() const;
		
		
	private:
		int m_file;
		int m_rank;
		int m_manPtr;
		int m_note;
};

#endif
