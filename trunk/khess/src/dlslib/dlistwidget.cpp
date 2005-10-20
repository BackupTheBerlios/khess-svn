/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                        	           *
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

#include "dlistwidget.h"

#include <qapplication.h>
#include "clicklineedit.h"
#include <qtoolbutton.h>
#include <qpainter.h>
#include <qsimplerichtext.h>

#include "erase.xpm"

DListWidget::DListWidget(QWidget *parent, const char *name) : QVBox(parent, name), m_filterText(tr("Filter here..." ))
{
	QHBox *searchToolBar = new QHBox( this );
	
	QToolButton *button = new QToolButton ( searchToolBar );
	button->setIconSet( QIconSet( (const char **) erase_xpm));
	m_searchEdit = new ClickLineEdit(m_filterText, searchToolBar );

// 	searchToolBar->setStretchableWidget( m_searchEdit );
	m_searchEdit->setFrame( QFrame::Sunken );

// 	m_searchEdit->setListView(m_listView);
	
	
	connect(button, SIGNAL(clicked()), m_searchEdit, SLOT(clear()));
	m_listView = new DListView(this);
}


DListWidget::~DListWidget()
{
}

// void DListWidget::makeList(QStringList colsText)
// {
// 	m_listView = new DListView(this);
// // 	QColor c; c.setRgb( 57, 64, 98 );
// // 	m_listView->setAlternateBackground( c );
// 	m_listView->setSelectionMode( QListView::Extended );
// 	m_listView->setAllColumnsShowFocus( true );
// 	
// 	m_listView->setMouseTracking(true);
// 
// 	for (uint i = 0; i < colsText.count(); i++)
// 	{
// 		m_listView->addColumn( colsText[i] );
// 	}
// 	
// 	connect( m_listView, SIGNAL(executed(QListViewItem* )), this, SLOT(getClickedItem(QListViewItem* )));
// }

// DListView
DListView::DListView(QWidget *parent, const char *name) : QListView(parent, name), m_title(""), m_explain("")
{
}

DListView::~ DListView()
{
}

void DListView::setTitle(const QString &t)
{
	m_title = t;
}

void DListView::setExplain(const QString &exp)
{
	m_explain = exp;
}

void DListView::viewportPaintEvent(QPaintEvent *e)
{
	QListView::viewportPaintEvent( e );

	if ( childCount() == 0 )
	{
		if ( !m_title.isEmpty() && !m_title.isEmpty())
		{
			QPainter p( viewport() );
	
			QSimpleRichText t( QString(
					"<div align=center>"
					"<h3>"+m_title+"</h3>"
					+m_explain+
					"</div>" ), QApplication::font() );
	
			t.setWidth( width() - 50 );
	
			const uint w = t.width() + 20;
			const uint h = t.height() + 20;
	
			p.setBrush( colorGroup().background() );
			p.drawRoundRect( 15, 15, w, h, (8*200)/w, (8*200)/h );
			t.draw( &p, 20, 20, QRect(), colorGroup() );
		}
	}
}

QString DListView::getText(int col)
{
	QString text = "";
	if ( currentItem() )
		text = currentItem()->text(col);
	return text;
}

// LTListVIewItem
DListViewItem::DListViewItem(QListView * parent) : QListViewItem(parent)
{
}

DListViewItem::DListViewItem(QListViewItem * parent) : QListViewItem(parent)
{
}

DListViewItem::~DListViewItem()
{
}

int DListViewItem::compare ( QListViewItem * i, int col, bool ascending )
{
	bool ok = false;
	
	int number = i->text(col).toInt(&ok);
	
	if (ok)
	{
		return number;
	}
	return QListViewItem::compare(i, col, ascending);
}

