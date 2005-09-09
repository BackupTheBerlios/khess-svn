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

#ifndef KHWARRIORBASE_H
#define KHWARRIORBASE_H

#include <qobject.h>
#include <qpixmap.h>
#include <qstring.h>

class KHWarriorBase;

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHWarriorBase : public QObject
{
	Q_OBJECT
	public:
		enum PlayerType
		{
			Human = 0,
			Engine,
			Internet
		};
		
		KHWarriorBase();
		~KHWarriorBase();
		void setInformation(const QString &name,const QString &login, PlayerType pt = Human );
		void setName(const QString &name);
		void setElo(int elo);
		void setAge(int age);
		void setType(PlayerType pt);
		
		QString theName() const;
		QString login() const;
		int elo() const;
		int age() const;
		int type() const;
		
	private:
		QPixmap m_defeatedFace, m_winFace;
		QString m_name, m_login;
		int m_elo;
		int m_age;
		bool m_isWhite;
		PlayerType m_type;
};

#endif
