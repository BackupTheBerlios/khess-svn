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

#include "khseektab.h"
#include <klocale.h>
#include <kdebug.h>

#include <cctype>
#include <ctime>

KHSeekTab::KHSeekTab(QWidget *parent) : LTListView(QStringList() << i18n("ID") << i18n("Rating") << i18n("Name") << i18n("Time") << i18n("Increment") << i18n("Rated") << i18n("Variant"), LTListView::ButtonAdd, LTListView::ButtonQuery, LTListView::NoButton, LTListView::NoButton, parent, "KHSeekTab")
{
	srand(time(0));
	setButtonText( ButtonAdd, i18n("Play!"));
	setButtonText( ButtonQuery, i18n("Refresh"));
}


KHSeekTab::~KHSeekTab()
{
}

void KHSeekTab::fillList()
{
	emit iWantPlay("sought all");
}

void KHSeekTab::addButtonClicked()
{
	getClickedItem(m_listView->currentItem());
}

void KHSeekTab::delButtonClicked()
{
}

void KHSeekTab::modifyButtonClicked()
{
}

void KHSeekTab::queryButtonClicked()
{
	fillList();
}

void KHSeekTab::getClickedItem(QListViewItem *item)
{
	if ( item )
	{
		kdDebug() << "Clicked: " << item->text(0) << endl;
		emit iWantPlay("play "+item->text(0));
	}
}

/*
 * 0: id
 * 1: elo
 * 2: login
 * 3: time
 * 4: increment
 * 5: is rated
 * 6: variant
 */
void KHSeekTab::pushSeeks(const QValueList<QStringList> &seeks)
{
// 	m_listView->clear();
	for(uint i = 0; i < seeks.count(); i++)
	{
		QStringList aSeek = seeks[i];
		QStringList::iterator it;
		
		LTListViewItem *item = new LTListViewItem(m_listView);
		
		int position = 0;
		for(it = aSeek.begin(); it != aSeek.end(); ++it)
		{
			if(position == 6)
			{
				if( *it == "standard" )
				{
					item->setText(position, i18n("standar"));
				}
				else if ( *it == "blitz")
				{
					item->setText(position, i18n("blitz"));
				}
			}
			else if (position == 5)
			{
				if(*it == "rated" )
				{
					item->setText(position, i18n("rated"));
				}
				else if (*it == "unrated")
				{
					item->setText(position, i18n("unrated"));
				}
			}
			else
			{
				item->setText(position, *it);
			}
			position++;
		}
	}
}

void KHSeekTab::showEvent(QShowEvent *e)
{
	kdDebug() << "Showed" << endl;
	m_listView->clear();
	fillList();
}


#include "khseektab.moc"
