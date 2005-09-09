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

#include "khplayersstatus.h"
#include <kmessagebox.h>
#include <krun.h>
#include <klocale.h>
#include <qlayout.h>

KHPlayersStatus::KHPlayersStatus(KHMatch *m, QWidget *parent, const char *name) : QVBox(parent, name), m_onTurn(true)
{
	setCaption(i18n("Status"));
	
	m_white = m->white()->face();
	m_black = m->black()->face();
	
	setIcon(m_white);
	
	m_whiteBox = new KHPlayerInfoBox(m->white(), this, m->clock()->displayLight());
	m_whiteBox->setPaletteBackgroundColor( Qt::gray);
	m_whiteBox->setPaletteForegroundColor( Qt::black);

	m_blackBox = new KHPlayerInfoBox(m->black(), this, m->clock()->displayBlack());
	m_blackBox->setPaletteBackgroundColor( Qt::black );
	m_blackBox->setPaletteForegroundColor( Qt::gray );
	
	connect(m->clock(), SIGNAL(playerChanged()), this, SLOT(updateIcon()));
	
}


KHPlayersStatus::~KHPlayersStatus()
{
}

void KHPlayersStatus::updateIcon()
{
	if ( !m_onTurn )
	{
		setIcon(m_white);
	}
	else
	{
		setIcon(m_black);
	}
	
	m_onTurn = !m_onTurn;
}


#include "khplayersstatus.moc"
