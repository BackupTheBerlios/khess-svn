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

#include "khhtmlpage.h"

#include <qpainter.h>
#include <qlayout.h>

#include <kurl.h>

#include <kmessagebox.h>
#include <krun.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <qtextcodec.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <ksavefile.h>
#include <kstandarddirs.h>
#include <kaction.h>
#include <kiconloader.h>

#include <assert.h>
#include <qfile.h>
#include <qdir.h>

KHHtmlPage::KHHtmlPage( QWidget *parentWidget, const char *widgetName, QObject *parent, const char *name ) : KHTMLPart( parentWidget, widgetName, parent, name, BrowserViewGUI ), m_parent(parentWidget)
{
	QTextCodec* codec = KGlobal::locale()->codecForEncoding();
	if (codec)
		setCharset(codec->name(), true);
	else
		setCharset("iso-8859-1", true);
        // about:blah isn't a kioslave -> disable View source
	KAction * act = actionCollection()->action("viewDocumentSource");
	if ( act )
		act->setEnabled( false );
	
	setOnlyLocalReferences(false);
	setPluginsEnabled(true);
	
	connect(this, SIGNAL(popupMenu(const QString &,const QPoint &)), this, SLOT(rightClicked(const QString &,const QPoint &)));
}

KHHtmlPage::~KHHtmlPage()
{
}

bool KHHtmlPage::openURL( const KURL &u )
{
	kdDebug() << "OPENING: " << u.url() << endl;
	kdDebug() << "HOSTNAME : " << u.host() << endl;
	m_history << u.url();
	return KHTMLPart::openURL(u);
}

bool KHHtmlPage::openFile()
{
	return true;
}

void KHHtmlPage::saveState( QDataStream &stream )
{
	stream << m_htmlDoc;
	stream << m_what;
}

void KHHtmlPage::restoreState( QDataStream &stream )
{
	stream >> m_htmlDoc;
	stream >> m_what;
	serve( m_htmlDoc, m_what );
}

void KHHtmlPage::serve( const QString& html, const QString& what )
{
	m_what = what;
	begin( KURL( QString("about:%1").arg(what) ) );
	write( html );
	end();
	m_htmlDoc = html;
}

void KHHtmlPage::urlSelected( const QString &url, int button, int state, const QString &target, KParts::URLArgs _args )
{
	qDebug(QString("url: %1").arg(url));
	
	QRegExp host("^(http://){0,1}([-\\w+\\.]+)(.*)");
	host.search(url);
	
	QStringList parts = host.capturedTexts();
	
	KURL u;
	
	if ( parts[1] == "http://" )
	{
		u.setProtocol("http");
		u.setHost(parts[2]);
		u.setPath(parts[3]);
	}
	else
	{
		u.setProtocol("http");
		u.setHost(baseURL().host());
		u.setPath("/"+url);
	}
	
	kdDebug() << "FIXED: " << u << endl;
	
	QString newPath = u.path();
	
	kdDebug() << "PATH SELECT: " << newPath << endl;
	
// 	u.setPath( encodeString(newPath ) );
	
	begin(u);
	openURL(u);
	
	qDebug(QString("Protocolo: %1").arg(u.protocol()));
	
	kdDebug() << "FINAL SELECTED: " << u.url() << endl;

// 	KHTMLPart::urlSelected( u.url(), button, state, target, _args );
}

void KHHtmlPage::rightClicked(const QString &url,const QPoint &point)
{
	if ( m_history.count() < 2 )
	{
		return;
	}
	
	QString oldUrl = m_history[m_history.count()-2];
	m_history.pop_back();
	
// 	kdDebug() << "OLD: " << oldUrl << endl;
	
	KHTMLPart::openURL(oldUrl);
}

QString KHHtmlPage::encodeString(QString toEncode)
{
// 	QTextCodec *codec = QTextCodec::
	
	QString result = "";
	
	for(uint i = 0; i < toEncode.length(); i++)
	{
		char var = toEncode[i].latin1();
		
		if ( var != '/' )
		{
			result+= QString("%%1").arg(var, 0, 16);
		}
		else
		{
			result+="/";
		}
	}
	
	kdDebug() << "Result: " << result << endl;
	
	return result;
}

#include "khhtmlpage.moc"
