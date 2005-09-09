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
#ifndef FIGURE_H
#define FIGURE_H

#include <qobject.h>
#include "khchessmove.h"

/**
	@author David Cuadrado <krawek@gmail.com >
*/
class Figure
{
	public:
		enum Type
		{
			None = 0,
			King,
			Queen,
			Rook,
			Knight,
			Bishop,
			Pawn
		};
		
		Figure();
		Figure(QChar piece);
		Figure(bool band, int type, int file, int rank);
		~Figure();
		
		QChar sanChar();
		
		void setPosition( const KHChessMove &move);
		
		void setBand(bool band);
		void setType(int type);
		void setFile(int file);
		void setRank(int rank);
		
		bool band();
		int type();
		int file();
		int rank();
		
	private:
		bool m_band; // true is white
		int m_type;
		int m_file;
		int m_rank;
};

#endif
