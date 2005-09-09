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

#include "khwarrior.h"
#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>


KHWarrior::KHWarrior() : KHWarriorBase(), m_ficsLogin("")
{
	kdDebug() << "[Initializing KHWarrior]" << endl;
}

KHWarrior::~KHWarrior()
{
	kdDebug() << "[Destroying KHWarrior]" << endl;
}

KHWarrior *KHWarrior::instance()
{
	static KHWarrior *tmp = new KHWarrior;
	
	return tmp;
}

void KHWarrior::setWarriorInformation(const QPixmap &px, const QString &login, const QString &name, const QString &type)
{
	setInformation(name, login);
	
	m_ficsLogin = login;
			
	m_face = QPixmap(px);
	
	if ( type == "Human")
	{
		setType(Human);
	}
	else if (type == "Engine")
	{
		setType(Engine);
	}
	else if (type == "Internet")
	{
		setType(Internet);
	}
	else
	{
		kdError() << "Bad player type!!" << endl;
	}
}

QPixmap KHWarrior::face() const
{
	return m_face;
}


void KHWarrior::setFicsLogin(const QString &login)
{
	m_ficsLogin = login;
}

QString KHWarrior::ficsLogin()
{
	return m_ficsLogin;
}

void KHWarrior::setSettings(const PlayerSettings &settings)
{
	m_settings = settings;
}

QString KHWarrior::setting(const QString &key)
{
	return m_settings[key];
}


#include "khwarrior.moc"
