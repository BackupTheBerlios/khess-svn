/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "khboardbase.h"

#include <kmessagebox.h>
#include <klocale.h>
#include <qpainter.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kio/netaccess.h>
#include <kconfig.h>
#include <ksimpleconfig.h>

#include <qimage.h>

#include "khwarrior.h"

KHBoardBase::KHBoardBase(KHMatch *m ,QWidget *parent, const char *name) : QWidget(parent, name), m_boardOrientation(true), m_game(0)
{
	kdDebug() << "[Initializing KHBoardBase]" << endl;
	
	bool local = true;
	
	if( m )
	{
		KHWarrior *black = m->black();
		KHWarrior *white = m->white();
		
		if ( PLAYER->login() == black->login() )
		{
			local = false;
		}
		else if ( PLAYER->login() == white->login() )
		{
			local = true;
		}
	}
	
	m_boardOrientation = local;
	
	m_game = new KHGame(m);
	
	connect(m_game, SIGNAL(promotion()), this, SLOT(showPromotionDialog()));
	kdDebug() << "[KHBoardBase Initialized]" << endl;
}


KHBoardBase::~KHBoardBase()
{
}

bool KHBoardBase::orientation()
{
	return m_boardOrientation;
}

void KHBoardBase::setLocalBand( bool band)
{
	if( band == m_boardOrientation )
	{
		return;
	}
	
	m_boardOrientation = band;
	redrawAll();
}

void KHBoardBase::showPromotionDialog()
{
	
}


#include "khboardbase.moc"
