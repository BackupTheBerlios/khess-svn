/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                   *
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

#ifndef DWIDGETBASE_H
#define DWIDGETBASE_H

#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qobjectlist.h>

class DWidgetTitle;

/**
 * @author David Cuadrado
*/
class DWidgetBase : public QWidget
{
	Q_OBJECT
	public:
		DWidgetBase(QWidget *parent, const char *name = 0);
		~DWidgetBase();
		virtual void addChild(QWidget * child );
		void setFont(const QFont &);
		
	private:
		QBoxLayout *m_container;
		QObjectList m_childs;
		bool m_isChildHidden;
		QWidget *m_originalParent;
		
	protected:
		DWidgetTitle *m_title;
		
	protected:
		virtual bool event( QEvent * e );
		
	public slots:
		void toggleView();
		void setCaption(const QString &text);
		
	protected:
		void enterEvent(QEvent *e);
		void leaveEvent(QEvent *e);
		
	signals:
		void documentModified(bool);
		void sendMessage(const QString &);
		void toggle();
		void activate(bool);
};

class DWidgetTitle : public QLabel
{
	Q_OBJECT
	public:
		DWidgetTitle(const QString &title, QWidget *parent = 0, const char *name = 0);
		~DWidgetTitle();
		
		
	public slots:
		void setText(const QString &text);
		
	signals:
		void doubleClicked();
		
	protected:
		void mouseDoubleClickEvent(QMouseEvent * e );

};

#endif
