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

#include "khselectuser.h"
#include <kstandarddirs.h>
#include <klocale.h>
#include <qpixmap.h>
#include <kdebug.h>
#include <kconfig.h>
#include <ksimpleconfig.h>

#include <cctype>
#include <ctime>

KHSelectUser::KHSelectUser(QWidget *parent) : KIconView(parent, "KHSelectUser")
{
	srand(time(0));
	
	m_currentWarrior = new KHWarrior;
	
	setItemsMovable ( false );
	setSelectionMode(QIconView::Single);
	setMinimumSize ( 300,100);
	
	m_playersDir.setPath ( locateLocal("data", "khess/players/") );
	
	readPlayers();
	connect(this, SIGNAL(clicked (QIconViewItem *)),this, SLOT(readCurrentPlayer(QIconViewItem *)));
}


KHSelectUser::~KHSelectUser()
{
}

KHWarrior *KHSelectUser::currentWarrior()
{
	return m_currentWarrior;
}

// Players are located in $(kde_localapp)/khess/players/
void KHSelectUser::readPlayers(bool onlyLocal)
{
	QStringList users = m_playersDir.entryList("*", QDir::Dirs | QDir::Readable | QDir::Writable );
	
	this->clear();
	
	for(uint i = 2; i < users.count(); i++)
	{
		if ( PLAYER->login() != users[i] )
		{
			KSimpleConfig userConfig( m_playersDir.path()+"/"+users[i]+"/player.conf" ,true );
			userConfig.setGroup("General");
			
			QString type = userConfig.readEntry("Type", "Human");
			
			if (onlyLocal)
			{
				if ( type == "Human" )
				{
					indexUser(users[i]);
				}
			}
			else
			{
				indexUser(users[i]);
			}
		}
	}
}

void KHSelectUser::indexUser(const QString &login)
{
	kdDebug() << "Indexing user: " << login << endl;
	
	QImage theFace(m_playersDir.path()+"/"+login+"/face.png");
	
	if ( theFace.isNull() )
	{
		int i = 1+(rand() % 3);
		theFace = QImage( locate("data",QString("khess/faces/default%1.png" ).arg( i ) )  );
	}
	
	QPixmap img( theFace.smoothScale(62,62) );
	
	KIconViewItem *userIcon = new KIconViewItem (this, login, img);
	userIcon->setDragEnabled (false);
	userIcon->setDropEnabled (false);
}

void KHSelectUser::readCurrentPlayer( QIconViewItem *item)
{
	if ( !item )
	{
		return;
	}
	
	QString login = item->text();
	
	kdDebug() << "Selected user: " << login << endl;
	KSimpleConfig userConfig( m_playersDir.path()+"/"+login+"/player.conf" ,true );
	userConfig.setGroup("General");
	
	QString name = userConfig.readEntry("Name", login );
	kdDebug() << "Name: " << name << endl;
	kdDebug() << "Age: " << userConfig.readEntry("Age", 0 ) << endl;
	
	
	// FIXME
	
	QString type = userConfig.readEntry("Type", "Human");
	
	m_currentWarrior->setWarriorInformation( *(item->pixmap()) ,login, name,type );
	
	m_currentWarrior->setElo( userConfig.readEntry("Elo", "1200").toInt() );
	
	if(type == "Engine" )
	{
		userConfig.setGroup("Engine");
		PlayerSettings settings;
		
		settings.insert("Command", userConfig.readEntry("Command", ""));
		settings.insert("Arguments", userConfig.readEntry("Arguments", ""));
		settings.insert("Depth", userConfig.readEntry("Depth", "0"));
		
		m_currentWarrior->setSettings(settings);
	}
	
			
	emit playerSelected();
}

#include "khselectuser.moc"
