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

#ifndef DLISTWIDGET_H
#define DLISTWIDGET_H

#include <qvbox.h>
#include <qhbuttongroup.h>
#include <qpushbutton.h>
#include <qlistview.h>

#include <qtoolbar.h>
#include <qtimer.h>
#include <qmessagebox.h>

class DListViewItem;
class DListView;

class ClickLineEdit;

/**
 * @author David Cuadrado
*/
class DListWidget : public QVBox
{
	Q_OBJECT
	public:
		DListWidget(QWidget *parent = 0, const char *name = 0);
		~DListWidget();
	
	private:
		ClickLineEdit *m_searchEdit;
		
	protected:
		DListView *m_listView;
		QString m_filterText;
};

class DListView : public QListView
{
	public:
		DListView(QWidget *parent=0, const char *name=0);
		~DListView();
		void viewportPaintEvent( QPaintEvent* );
		void setTitle(const QString &t);
		void setExplain(const QString &exp);
		QString getText(int col);
		
	private:
		QString m_title;
		QString m_explain;
};

class DListViewItem : public QListViewItem
{
	public:
		DListViewItem(QListView * parent);
		DListViewItem(QListViewItem * parent);
		~DListViewItem();
		int compare ( QListViewItem * i, int col, bool ascending );
};



#endif
