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

#include "khwebbrowser.h"
#include <qlabel.h>
#include <klocale.h>
#include <kdebug.h>
#include <kurl.h>
#include <qtextcodec.h>
#include <qregexp.h>

using namespace KNetwork;

KHWebBrowser::KHWebBrowser( QWidget *parent) : QVBox(parent, "WebBrowser")
{
	setCaption (i18n("Web Browser"));
	(new QVBoxLayout(this))->setAutoAdd(true);
	
	QHBox *addressBox = new QHBox(this);
	new QLabel(i18n("Address: "), addressBox);
	m_address = new KLineEdit(addressBox);
	
	m_page = new KHHtmlPage(this, "browser", this, "browser");
	
	connect(m_address, SIGNAL(returnPressed(const QString &)), this, SLOT(resolve(const QString &)));
}

KHWebBrowser::~KHWebBrowser()
{
}

void KHWebBrowser::resolve(const QString &u)
{
	KURL url;
	
	QString urlStr = u;
	
	url.setProtocol("http");
	if ( ! u.startsWith("http://") )
	{
		urlStr = "http://"+u;
	}

	QRegExp host("^http://([-\\w+\\.]+)+(.*)");
	host.search(urlStr);

	url.setHost(host.cap(1));
	url.setPath(host.cap(2));
	
	if( url.isValid() )
	{
		kdDebug() << "URL VALIDO" << endl;
		
		kdDebug() << "HOST: " << url.host() << endl;
		kdDebug() << "PATH: " << url.path() << endl;
		
		KResolverResults results = KResolver::resolve(url.host() , QString::null);
		
		for(uint i = 0; i < results.count(); i++)
		{
			kdDebug() << "ip " << results[i].address().nodeName() << endl;
		}
		if  (results.count() > 0)
		{
			QString urlFinal = "http://"+results[0].address().nodeName()+m_page->encodeString(url.path());
			
			kdDebug() << "URLFINAL: " << urlFinal << endl;
			
			m_page->openURL(urlFinal);
		}
	}
	else
	{
		kdDebug() << "URL INVALIDO" << endl;
	}
}


#include "khwebbrowser.moc"
