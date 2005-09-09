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

#include "khconsole.h"
#include <qlabel.h>
#include <klocale.h>
#include <kdebug.h>

KHConsole::KHConsole(QWidget *parent) : QVBox(parent, "KHConsole")
{
	m_edit = new KTextEdit(this);
	m_edit->setWordWrap( KTextEdit::NoWrap);
	m_edit->setReadOnly(true);
	m_edit->setPaper( QBrush(Qt::black) );
	m_edit->setTextFormat( Qt::RichText );
	m_edit->setPaletteForegroundColor( Qt::gray );
	
// 	m_edit->setText("<font color=red>initializing... fds afdsa fdsa fdsa fdsafdsa fdsa</color>");
	
	QHBox *pBox = new QHBox(this);
	
	new QLabel(i18n("Command"), pBox);
	
	m_prompt = new KLineEdit(pBox);
	
	connect(m_prompt, SIGNAL(returnPressed(const QString &)), this, SLOT(sendCommand(const QString &)));
	
	m_bannedWords << "login:" << "password:";
	// 	m_highlightWords << "seek" << ""; // El login
}


KHConsole::~KHConsole()
{
}

void KHConsole::sendCommand(const QString &command)
{
	kdDebug() << "Sending command: " << command << endl;
	
	emit tryExec(command);
	
	m_prompt->clear();
}

void KHConsole::setText(const QString &data)
{
	QStringList list = QStringList::split('\n', data);
	
	for(uint i = 0; i < list.count(); i++ )
	{
		for (uint j = 0; j < m_bannedWords.count(); j++)
		{
			if (data.startsWith(m_bannedWords[j]) )
			{
				m_edit->append("<br>");
				break;
			}
		}
		
		if ( !list[i].isEmpty() )
		{
			m_edit->append(list[i]);
		}
	}
}


#include "khconsole.moc"
