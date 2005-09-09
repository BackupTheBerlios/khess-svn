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

#include "khtextbox.h"
#include <qregexp.h>

#include <kdebug.h>
#include <qstylesheet.h>

#include "khwarrior.h"

KHTextBox::KHTextBox(QWidget *parent, const char *name) : KTextBrowser(parent, name)
{
	m_highlighColor = Qt::red;
}


KHTextBox::~KHTextBox()
{
}

void KHTextBox::append (const QString & text)
{
	QRegExp http("((http://)|(www.)\\w+\\.\\w+(\\.\\w*)*)");
	
	QString newText = QStyleSheet::escape(text);
	
	QStringList words = QStringList::split(QRegExp("\\s+"), newText);
	
	QStringList::iterator it;
	
	bool highlightLine = false;
	bool boldLine = false;
	
	for(it = words.begin(); it != words.end(); ++it)
	{
// 		kdDebug() << "." << *it <<"."<< endl;
		
		if ( http.search((*it)) > -1 )
		{
			if ( (*it).startsWith("www"))
			{
				newText.replace((*it), QString("<a href=http://%1>%2</a>").arg((*it)).arg((*it)));
			}
			else
			{
				newText.replace((*it), QString("<a href=%1>%2</a>").arg((*it)).arg((*it)));
			}
		}
		else if ( (*it) == PLAYER->ficsLogin() && ! ((*it) == "&lt;"+PLAYER->ficsLogin()+"&gt;"))
		{
			highlightLine = true;
		}
		else if ( (*it) == "&lt;"+PLAYER->ficsLogin()+"&gt;")
		{
			boldLine = true;
		}
	}
	
	if ( highlightLine )
	{
		newText = QString("<font color=%1>").arg(m_highlighColor.name())+newText+"</font>";
	}
	if (boldLine)
	{
		newText = "<b>"+newText+"</b>";
	}
	
// 	kdDebug() << newText << endl;
	
	if( newText[newText.length()] != '\n' )
	{
		newText += "\n";
	}
	
	QTextEdit::append(newText);
}

void KHTextBox::setHighlightColor(const QColor &color)
{
	m_highlighColor = color;
}

// QPopupMenu *createPopupMenu ( const QPoint & pos )
// {
// 	
// }
	


#include "khtextbox.moc"
