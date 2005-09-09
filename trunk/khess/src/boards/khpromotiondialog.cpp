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

#include "khpromotiondialog.h"
#include <klocale.h>
#include <qlayout.h>

KHPromotionDialog::KHPromotionDialog(KHImageManager *imageManager, bool isWhite, QWidget *parent) : KDialog(parent, "KHPromotionDialog")
{
	(new QVBoxLayout(this))->setAutoAdd(true);
	
	m_buttonGroup = new QHButtonGroup(i18n("Promotion"), this);
	
	if ( isWhite )
	{
		m_rook = new KPushButton( QPixmap(imageManager->get(KHImageManager::WhiteRook, 64 )), "", m_buttonGroup );
		m_knight = new KPushButton(QPixmap( imageManager->get(KHImageManager::WhiteKnight, 64 )), "", m_buttonGroup );
		m_bishop = new KPushButton(QPixmap( imageManager->get(KHImageManager::WhiteBishop, 64 )), "", m_buttonGroup );
		m_queen = new KPushButton(QPixmap( imageManager->get(KHImageManager::WhiteQueen, 64 )), "", m_buttonGroup );
	}
	else
	{
		m_rook = new KPushButton(QPixmap( imageManager->get(KHImageManager::BlackRook, 64 ) ), "", m_buttonGroup);
		m_knight = new KPushButton(QPixmap( imageManager->get(KHImageManager::BlackKnight, 64 ) ), "", m_buttonGroup);
		m_bishop = new KPushButton(QPixmap( imageManager->get(KHImageManager::BlackBishop, 64 ) ), "", m_buttonGroup);
		m_queen = new KPushButton(QPixmap( imageManager->get(KHImageManager::BlackQueen, 64 ) ), "", m_buttonGroup);
	}
	
	m_buttonGroup->insert(m_rook, 'r');
	m_buttonGroup->insert(m_knight, 'n');
	m_buttonGroup->insert(m_bishop, 'b');
	m_buttonGroup->insert(m_queen, 'q');
	
	connect(m_buttonGroup, SIGNAL(clicked(int)), this, SLOT(done(int)));
}


KHPromotionDialog::~KHPromotionDialog()
{
}


#include "khpromotiondialog.moc"
