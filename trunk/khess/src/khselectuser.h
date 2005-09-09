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

#ifndef KHSELECTUSER_H
#define KHSELECTUSER_H

#include <kiconview.h>
#include <qdir.h>

#include "khwarrior.h"

/**
 * @author David Cuadrado <krawek@gmail.com >
 * @TODO: Move to lib
*/

class KHSelectUser : public KIconView
{
	Q_OBJECT
	public:
		KHSelectUser(QWidget *parent = 0 );
		~KHSelectUser();
		KHWarrior *currentWarrior();
		
	public slots:
		void readPlayers(bool onlyLocal = false);
		void indexUser(const QString &login);
		void readCurrentPlayer(QIconViewItem *item);
		
		
	signals:
		void playerSelected();
		
	private:
		QDir m_playersDir;
		KHWarrior *m_currentWarrior;

};

#endif
