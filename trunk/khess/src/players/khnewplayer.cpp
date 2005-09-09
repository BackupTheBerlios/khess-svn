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

#include "khnewplayer.h"
#include <klocale.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kio/job.h>
#include <qfile.h>
#include <ksimpleconfig.h>

KHNewPlayer::KHNewPlayer(QWidget *parent) : KDialogBase(parent, "KHNewPlayer", true, i18n("Create a new player"), Ok | Cancel, Ok,true)
{
	QVBox *page = makeVBoxMainWidget();
	page->setMargin(5);
	page->setLineWidth(4);
	page->setMidLineWidth(4);
	page->setFrameStyle (QFrame::Box | QFrame::Raised );

	QHGroupBox *personalInfo = new QHGroupBox(i18n("Personal information"), page);
	
	m_face = new KHSelectFaceButton(personalInfo);
	
	QVBox *vbox1 = new QVBox(personalInfo);
	
	QHBox *firstLine = new QHBox(vbox1);
	new QLabel( i18n("Name"), firstLine );
	
	new QLabel(i18n("Age"), firstLine);
	
	QHBox *secondLine = new QHBox(vbox1);
	m_nameLE = new KLineEdit( secondLine );
	
	m_ageBox = new KIntNumInput( 18, secondLine);
	m_ageBox->setRange(0, 100, 1, true);
	
	QHBox *thirthLine = new QHBox(vbox1);
	new QLabel( i18n("Login"), thirthLine );
	new QLabel(i18n("ELO"), thirthLine);
	
	
	QHBox *fourthLine = new QHBox(vbox1);
	m_loginLE = new KLineEdit( fourthLine );

	m_eloBox = new KIntNumInput( 0, fourthLine);
	m_eloBox->setRange(0, 3000, 1, true);
	
	setMaximumSize( sizeHint());
}

KHNewPlayer::~KHNewPlayer()
{
}

void KHNewPlayer::slotOk()
{
	QString directoryPath = "khess/players/"+m_loginLE->text()+"/";
	kdDebug() << "Making directory: " <<  directoryPath << endl;
	if ( ! m_loginLE->text().isEmpty() )
	{
		QString directory = ::locateLocal( "data", directoryPath, true);
		
		if ( QFile::exists(directory+"/player.conf") )
		{
			kdDebug() << "ERROR: login exists!" << endl;
			return;
		}
		
		kdDebug() << "Creating: " << directory << endl;
		
		KSimpleConfig *m_userConfig = new KSimpleConfig(directory+"/player.conf", false);
		
		m_userConfig->setGroup("General");
		m_userConfig->writeEntry("Name", m_nameLE->text());
		m_userConfig->writeEntry("Age", m_ageBox->value() );
		m_userConfig->writeEntry("Elo", m_eloBox->value() );
		m_userConfig->writeEntry("Type", "Human" );
		
		m_userConfig->sync();
		
// 		delete m_userConfig;
		
		m_face->pixmap()->save(directory+"/face.png", "PNG");
		
	}
	else
	{
		kdDebug() << "Error" << endl;
	}
	
	KDialogBase::slotOk();
}


#include "khnewplayer.moc"
