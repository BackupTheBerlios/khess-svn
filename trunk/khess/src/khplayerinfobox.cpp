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

#include "khplayerinfobox.h"
#include "khclock.h"

#include <qlayout.h>
#include <qvbox.h>

KHPlayerInfoBox::KHPlayerInfoBox(const KHWarrior *warrior, QWidget *parent, KHSimpleClock *clock) : QFrame(parent, "KHPlayerInfoBox"), m_warrior(warrior)
{
	QVBoxLayout *m_layout = new QVBoxLayout(this);
	m_layout->setSpacing(3);
	
	setLineWidth(2);
	setMidLineWidth(2);
	setFrameStyle( QFrame::Box );
	setMargin(1);
	
	QVBox *m_info = new QVBox(this);
	
	m_name = new QLabel(QString("<h5>")+warrior->name()+QString("</h5>"), m_info);
	
	m_login = new QLabel(QString("<u>")+warrior->login()+ " ("+QString::number(warrior->elo())+")"+QString("</u>"), m_info );
	
	m_name->setBuddy(m_login);
	m_login->setBuddy(m_name);
	
	m_layout->addWidget(m_info, 1, Qt::AlignCenter);
	
	m_face = new QLabel(m_info);
	m_face->setLineWidth(2);
	m_face->setMidLineWidth(2);

	m_face->setFrameStyle( QFrame::Box );
	
// 	m_face->setBuddy(m_info);
// 	m_layout->addWidget(m_face, 0, Qt::AlignCenter);
	
	if ( clock )
	{
		clock->reparent(this, QPoint(0,0), true);
		m_layout->addWidget(clock, 1, Qt::AlignCenter);
	}
}


KHPlayerInfoBox::~KHPlayerInfoBox()
{
}

void KHPlayerInfoBox::showEvent(QShowEvent *e)
{
	QPixmap tmp(m_warrior->face());
	m_face->setMaximumSize(tmp.size()+QSize(5,5));
	m_face->setPixmap(tmp);
	
	m_name->setText( QString("<h5>")+m_warrior->theName()+QString("</h5>") );
	m_login->setText( QString("<em><u>")+m_warrior->login()+ " ("+QString::number(m_warrior->elo())+")"+QString("</u></em>") );
}


#include "khplayerinfobox.moc"
