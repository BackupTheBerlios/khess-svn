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

#include "ltlistview.h" 
#include <klocale.h>
#include <kapplication.h>
#include "clicklineedit.h"
#include <ktoolbarbutton.h>
#include <qpainter.h>
#include <qsimplerichtext.h>
#include <kdebug.h>

LTListView::LTListView(QWidget *parent, const char *name) : QVBox(parent, name), m_buttonAdd(0), m_buttonDel(0), m_buttonModify(0), m_buttonQuery(0)
{
}

LTListView::LTListView(QStringList colsText, Button button1, Button button2, Button button3, Button button4, QWidget *parent, const char *name) : QVBox(parent, name), m_buttonAdd(0), m_buttonDel(0), m_buttonModify(0), m_buttonQuery(0), m_filterText(i18n("Filter here..." ))
{
	KToolBar *searchToolBar = new KToolBar( this );
	
	KToolBarButton *button = new KToolBarButton( "locationbar_erase", 0, searchToolBar );
	m_searchEdit = new ClickLineEdit(m_filterText, searchToolBar );

	searchToolBar->setStretchableWidget( m_searchEdit );
	m_searchEdit->setFrame( QFrame::Sunken );

 	makeList(colsText);
	makeButtons(button1, button2, button3, button4);
	
	m_searchEdit->setListView(m_listView);
	
}


LTListView::~LTListView()
{
}

void LTListView::makeList(QStringList colsText)
{
	m_listView = new KLListView(this);
// 	QColor c; c.setRgb( 57, 64, 98 );
// 	m_listView->setAlternateBackground( c );
	m_listView->setSelectionMode( QListView::Extended );
	m_listView->setAllColumnsShowFocus( true );
	
	m_listView->setMouseTracking(true);
	
#if KDE_IS_VERSION( 3, 3, 91 )
    	m_listView->setShadeSortColumn( true );
#endif

	for (uint i = 0; i < colsText.count(); i++)
	{
		m_listView->addColumn( colsText[i] );
	}
	
	connect( m_listView, SIGNAL(executed(QListViewItem* )), this, SLOT(getClickedItem(QListViewItem* )));
}

void LTListView::makeButtons(Button button1, Button button2, Button button3, Button button4)
{
	m_buttons = new QHButtonGroup(this);
	switch ( button1 )
	{
		case ButtonAdd:
		{
			m_buttonAdd = new KPushButton(i18n("Add"), m_buttons);
			m_buttons->insert( m_buttonAdd, button1);
		}
		break;
		
		case ButtonDel:
		{
			m_buttonDel = new KPushButton(i18n("Delete"), m_buttons);
			m_buttons->insert( m_buttonDel, button1);
		}
		break;
		
		case ButtonModify:
		{
			m_buttonModify = new KPushButton(i18n("Modify"), m_buttons);
			m_buttons->insert( m_buttonModify, button1);
		}
		break;
		
		case ButtonQuery:
		{
			m_buttonQuery = new KPushButton(i18n("Query"), m_buttons);
			m_buttons->insert( m_buttonQuery, button1);
		}
		break;
		
		case NoButton:
		{
		}
		break;
	}
	
	switch (button2)
	{
		case ButtonAdd:
		{
			m_buttonAdd = new KPushButton(i18n("Add"), m_buttons);
			m_buttons->insert( m_buttonAdd, button2);
		}
		break;
		
		case ButtonDel:
		{
			m_buttonDel = new KPushButton(i18n("Delete"), m_buttons);
			m_buttons->insert( m_buttonDel, button2);
		}
		break;
		
		case ButtonModify:
		{
			m_buttonModify = new KPushButton(i18n("Modify"), m_buttons);
			m_buttons->insert( m_buttonModify, button2);
		}
		break;
		
		case ButtonQuery:
		{
			m_buttonQuery = new KPushButton(i18n("Query"), m_buttons);
			m_buttons->insert( m_buttonQuery, button2);
		}
		break;
		
		case NoButton:
		{
		}
		break;
	}
	
	switch(button3)
	{
		case ButtonAdd:
		{
			KPushButton *m_buttonAdd = new KPushButton(i18n("Add"), m_buttons);
			m_buttons->insert( m_buttonAdd, button3);
		}
		break;
		
		case ButtonDel:
		{
			m_buttonDel = new KPushButton(i18n("Delete"), m_buttons);
			m_buttons->insert( m_buttonDel, button3);
		}
		break;
		
		case ButtonModify:
		{
			m_buttonModify = new KPushButton(i18n("Modify"), m_buttons);
			m_buttons->insert( m_buttonModify, button3);
		}
		break;
		
		case ButtonQuery:
		{
			m_buttonQuery = new KPushButton(i18n("Query"), m_buttons);
			m_buttons->insert( m_buttonQuery, button3);
		}
		break;
		
		case NoButton:
		{
		}
		break;
	}
	
	switch(button4)
	{
		case ButtonAdd:
		{
			m_buttonAdd = new KPushButton(i18n("Add"), m_buttons);
			m_buttons->insert( m_buttonAdd, button4);
		}
		break;
		
		case ButtonDel:
		{
			m_buttonDel = new KPushButton(i18n("Delete"), m_buttons);
			m_buttons->insert( m_buttonDel, button4);
		}
		break;
		
		case ButtonModify:
		{
			m_buttonModify = new KPushButton(i18n("Modify"), m_buttons);
			m_buttons->insert( m_buttonModify, button4);
		}
		break;
		
		case ButtonQuery:
		{
			m_buttonQuery = new KPushButton(i18n("Query"), m_buttons);
			m_buttons->insert( m_buttonQuery, button4);
		}
		break;
		
		case NoButton:
		{
		}
		break;
	}
	
	if ( m_buttonAdd )
		connect(m_buttonAdd, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
	if (m_buttonDel )
		connect(m_buttonDel, SIGNAL(clicked()), this, SLOT(delButtonClicked()));
	if (m_buttonModify)
		connect(m_buttonModify, SIGNAL(clicked()), this, SLOT(modifyButtonClicked()));
	if(m_buttonQuery)
		connect(m_buttonQuery, SIGNAL(clicked()), this, SLOT(queryButtonClicked()));
}

void LTListView::setButtonText(Button b, const QString &text)
{
	m_buttons->find(b)->setText(text);
}

void LTListView::putItems(QStringList items)
{
	KListViewItem *itemp = new KListViewItem(m_listView);
	for (uint i = 0; i < items.count(); i++)
	{
		if( items[i] == "true")
		{
			itemp->setText(i, i18n("yes"));
		}
		else if (items[i] == "false")
		{
			itemp->setText(i, i18n("no"));
		}
		else
		{
			itemp->setText(i, items[i]);
		}
	}
	m_listView->insertItem(itemp);
}

void LTListView::addItem(const QString &pkey)
{
	// TODO: Esto sera virtual puro
	qDebug(QString("%1:%2 Por favor reimplemente la funcion \"addItem\" en su respectivo modulo!!").arg(__FILE__).arg(__LINE__) );
}

KListView* LTListView::getListView()
{
	return m_listView;
}

// KLListView
KLListView::KLListView(QWidget *parent, const char *name) : KListView(parent, name), m_title(""), m_explain("")
{
}

KLListView::~ KLListView()
{
}

void KLListView::setTitle(const QString &t)
{
	m_title = t;
}

void KLListView::setExplain(const QString &exp)
{
	m_explain = exp;
}

void KLListView::viewportPaintEvent(QPaintEvent *e)
{
	KListView::viewportPaintEvent( e );

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

QString KLListView::getText(int col)
{
	QString text = "";
	if ( currentItem() )
		text = currentItem()->text(col);
	return text;
}

// LTListVIewItem
LTListViewItem::LTListViewItem(QListView * parent) : KListViewItem(parent)
{
}

LTListViewItem::LTListViewItem(QListViewItem * parent) : KListViewItem(parent)
{
}

LTListViewItem::~ LTListViewItem()
{
}

int LTListViewItem::compare ( QListViewItem * i, int col, bool ascending )
{
	kdDebug() << "****************CMP******************" << endl;
	bool ok = false;
	
	int number = i->text(col).toInt(&ok);
	
	if (ok)
	{
		return number;
	}
	return QListViewItem::compare(i, col, ascending);
}

#include "ltlistview.moc"
