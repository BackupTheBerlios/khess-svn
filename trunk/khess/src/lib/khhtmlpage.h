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

#ifndef KHHTMLPAGE_H
#define KHHTMLPAGE_H

#include <kmdichildview.h>
#include <qlabel.h>
#include <khtml_part.h>

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHHtmlPage : public KHTMLPart
{
	Q_OBJECT
	public:
		KHHtmlPage(QWidget *parentWidget, const char *widgetName,
			   QObject *parent, const char *name );
		~KHHtmlPage();

		virtual bool openURL( const KURL &url );
		virtual bool openFile();
		virtual void saveState( QDataStream &stream );
		virtual void restoreState( QDataStream &stream );
		
		QString encodeString(QString toEncode);
		
	private slots:
		void rightClicked(const QString &,const QPoint &);

	protected:
		virtual void urlSelected( const QString &url, int button, int state, const QString &target, KParts::URLArgs args = KParts::URLArgs() );
		
	private:
		void serve( const QString&, const QString& );

		KHTMLPart *m_doc;
		QString m_htmlDoc;
		QString m_what;
		QWidget *m_parent;
		
		QStringList m_history;
};

#endif
