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

#include "figure.h"

Figure::Figure() : m_band(true), m_type(-1), m_file(-1), m_rank(-1)
{
}

Figure::Figure(QChar piece) : m_band(true), m_type(-1), m_file(-1), m_rank(-1)
{
	switch( piece.lower() )
	{
		case 'k':
		{
			if( piece == 'K' )
			{
				m_band = true;
			}
			else
			{
				m_band = false;
			}
			m_type = King;
			break;
		}
		case 'q':
		{
			if( piece == 'Q' )
			{
				m_band = true;
			}
			else
			{
				m_band = false;
			}
			m_type = Queen;
		}
		break;
		case 'b':
		{
			if( piece == 'B' )
			{
				m_band = true;
			}
			else
			{
				m_band = false;
			}
			m_type = Bishop;
			break;
		}
		case 'n':
		{
			if( piece == 'N' )
			{
				m_band = true;
			}
			else
			{
				m_band = false;
			}
			m_type = Knight;
		}
		break;
		case 'r':
		{
			if( piece == 'R' )
			{
				m_band = true;
			}
			else
			{
				m_band = false;
			}
			m_type = Rook;
		}
		break;
		case 'p':
		{
			if( piece == 'P' )
			{
				m_band = true;
			}
			else
			{
				m_band = false;
			}
			m_type = Pawn;
		}
		break;
		default:
		{
			m_type = None;
		}
		break;
	}
}

Figure::Figure(bool band, int type, int file, int rank) : m_band(band), m_type(type), m_file(file), m_rank(rank)
{
}

Figure::~Figure()
{
}

void Figure::setPosition( const KHChessMove &move)
{
	m_file = move.toFile();
	m_rank = move.toRank();
}

void Figure::setBand(bool band)
{
	m_band = band;
}

void Figure::setType(int type)
{
	m_type = type;
}

void Figure::setFile(int file)
{
	m_file = file;
}

void Figure::setRank(int rank)
{
	m_rank = rank;
}

bool Figure::band()
{
	return m_band;
}

int Figure::type()
{
	return  m_type;
}

int Figure::file()
{
	return m_file;
}

int Figure::rank()
{
	return m_rank;
}

QChar Figure::sanChar()
{
	QChar SAN;
	
	switch(m_type)
	{
		case Figure::Queen:
		{
			SAN = 'Q';
			break;
		}
		case Figure::King:
		{
			SAN = 'K';
			break;
		}
		case Figure::Bishop:
		{
			SAN = 'B';
			break;
		}
		case Figure::Knight:
		{
			SAN = 'N';
			break;
		}
		case Figure::Rook:
		{
			SAN = 'R';
			break;
		}
		case Figure::Pawn:
		{
			SAN = 'P';
			break;
		}
		default:
		{
			break;
		}
	}

	return SAN;
}
