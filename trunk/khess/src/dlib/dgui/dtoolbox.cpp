/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dtoolbox.h"

#include <QPolygon>
#include <QPainter>

DToolBox::DToolBox(QWidget *parent)
	: QToolBox(parent), m_lastIndex(0)
{
	setFrameStyle(QFrame::StyledPanel );
	
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(changeIcon(int)));
	
	createIcon();
}


DToolBox::~DToolBox()
{
}

void DToolBox::createIcon()
{
	QPolygon m_pArrow(3);

	QPixmap pixmap (22,22);
	
	m_pArrow.setPoint ( 0, 0, 0);
	m_pArrow.setPoint ( 1, 10, 5);
	m_pArrow.setPoint ( 2, 0, 10);
	
	QPainter p(&pixmap);
	
	p.fillRect(pixmap.rect(), palette().background() );
	p.setBrush( palette().foreground() );
	p.translate(pixmap.width()/3, 5);
	p.drawConvexPolygon(m_pArrow);

	p.end();
	
	m_icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
	
	///////////
	
	QPixmap pixmap2 (22,22);
	
	m_pArrow.setPoint ( 0, 0, 0);
	m_pArrow.setPoint ( 1, 5, 10);
	m_pArrow.setPoint ( 2, 10, 0);
	
	QPainter p2(&pixmap2);
	
	p2.fillRect(pixmap2.rect(), palette().background() );
	p2.setBrush( palette().foreground() );
	p2.translate(pixmap2.width()/3, 5);
	p2.drawConvexPolygon(m_pArrow);

	p2.end();
	
	m_icon.addPixmap(pixmap2, QIcon::Normal, QIcon::On);
	
}

void DToolBox::changeIcon(int index)
{
	setItemIcon(m_lastIndex, m_icon.pixmap (QSize(22,22), QIcon::Normal, QIcon::Off));
	
	setItemIcon(index, m_icon.pixmap (QSize(22,22), QIcon::Normal, QIcon::On));
	
	m_lastIndex = index;
}

void DToolBox::addPage(QWidget *page, const QString &title)
{
	addItem(page, m_icon, title);
}

