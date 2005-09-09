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

#include "khnicklist.h"
#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>

KHNickList::KHNickList(QWidget *parent, const char *name)
 : QVBox(parent, name)
{
	m_header = new QHeader(this);
	m_nickList = new KListBox(this);
	connect(m_nickList, SIGNAL(contextMenuRequested(QListBoxItem *, const QPoint & )), this, SIGNAL(contextMenu(QListBoxItem *, const QPoint & )));
	
	m_header->addLabel(SmallIcon("edu_languages"), i18n("Nicks"));
}


KHNickList::~KHNickList()
{
}

QString KHNickList::currentNick()
{
	return m_nickList->currentText();
}

void KHNickList::setNicks(const QStringList &nicks)
{
	m_nickList->insertStringList(nicks);
}

void KHNickList::showEvent(QShowEvent *)
{
	m_header->resizeSection(0, width());
}

#include "khnicklist.moc"
