/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado - krawek@gmail.com               *
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

#ifndef _KHPLAYERSSTATUS_H_
#define _KHPLAYERSSTATUS_H_

#include <kmdichildview.h>
#include "khplayerinfobox.h"
#include <qvbox.h>
#include "khmatch.h"
#include "khclock.h"

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHPlayersStatus : public QVBox
{
	Q_OBJECT
	public:
		KHPlayersStatus(KHMatch *m, QWidget *parent = 0, const char *name = 0);
		~KHPlayersStatus();
		
	private slots:
		void updateIcon();
		
	private:
		KHPlayerInfoBox *m_whiteBox, *m_blackBox;
		QPixmap m_white, m_black;
		bool m_onTurn;
};

#endif
