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
 
#include "position.h"

Position::Position(): m_file(-1), m_rank(-1), m_manPtr(-1), m_note(NOTE_NONE)
{
}

Position::Position(int file, int rank, int manptr, NoteType note) : m_file(file), m_rank(rank), m_manPtr(manptr), m_note(note)
{
}

Position::~Position()
{
}

void Position::setFile(int file)
{
	m_file = file;
}

void Position::setRank(int rank)
{
	m_rank = rank;
}

void Position::setManPtr(int mnptr)
{
	m_manPtr = mnptr;
}

void Position::setNote (NoteType note)
{
	m_note = note;
}

int Position::file() const
{
	return m_file ;
}

int Position::rank() const
{
	return m_rank ;
}
int Position::manPtr() const
{
	return  m_manPtr;
}
int Position::note() const
{
	return m_note ;
}
