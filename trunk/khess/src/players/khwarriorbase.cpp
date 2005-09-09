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

#include "khwarriorbase.h"
#include <kdebug.h>

KHWarriorBase::KHWarriorBase() : m_name("Unnamed"), m_login("..."), m_elo(0), m_age(0)
{
}

KHWarriorBase::~KHWarriorBase()
{
}

void KHWarriorBase::setInformation(const QString &name,const QString &login, PlayerType pt)
{
	m_name = name;
	m_login = login;
	setType(pt);
}

void KHWarriorBase::setName(const QString &name)
{
	m_name = name;
}

void KHWarriorBase::setElo(int elo)
{
	m_elo = elo;
}

void KHWarriorBase::setAge(int age)
{
	m_age = age;
}

void KHWarriorBase::setType( PlayerType pt)
{
	// TODO: Load the defaults images for m_defeatedFace and m_winFace
	m_type = pt;
	switch(m_type)
	{
		case Human:
		{
		}
		break;
		case Engine:
		{
		}
		break;
		case Internet:
		{
		}
		break;
	}
}

QString KHWarriorBase::theName() const
{
	return m_name ;
}

QString KHWarriorBase::login() const
{
	return m_login ;
}

int KHWarriorBase::elo() const
{
	return m_elo ;
}

int KHWarriorBase::age() const
{
	return m_age ;
}

int KHWarriorBase::type() const
{
	return m_type ;
}


#include "khwarriorbase.moc"
