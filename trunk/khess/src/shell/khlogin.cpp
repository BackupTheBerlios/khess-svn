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

#include "khlogin.h"
#include <kstandarddirs.h>
#include <klocale.h>
#include <qlayout.h>
#include <qvbox.h>
#include <kseparator.h>
#include <kmessagebox.h>
#include <kiconloader.h>

#include "khwarrior.h"

KHLogin::KHLogin(QWidget *parent ) : KDialogBase(parent, "KHLogin", true, i18n("Log in"), User1 |KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, true)
{
	QVBox *page = makeVBoxMainWidget();
	page->setMargin(5);
	page->setLineWidth(4);
	page->setMidLineWidth(4);
	page->setFrameStyle (QFrame::Box | QFrame::Raised );
	
	QVGroupBox *m_container = new QVGroupBox(i18n("Select your player"),page);
	
	m_selector = new KHSelectUser(m_container);
	m_selector->readPlayers(true);
	connect(m_selector, SIGNAL(playerSelected()), this, SLOT(setPlayerInformation()));
	
	QVBox *m_infoBox = new QVBox(m_container);
	
	m_labelName = new QLabel(i18n("Name: "), m_container);
	m_labelElo = new QLabel(i18n("Elo: "), m_container); 
	m_labelType = new QLabel(i18n("Type: "), m_container);
	
	new KSeparator(m_container);
	
	new QLabel(i18n("Login"), m_container);
	m_login = new KLineEdit(m_container);
	new QLabel(i18n("Password"), m_container);
	m_password = new KLineEdit(m_container);
	
	setModal(true);
	
	setButtonGuiItem (KDialogBase::User1, KGuiItem( i18n("Create a new user"), SmallIcon("penguin"), i18n("Click to create new user"), i18n("Clicking this button you can create a new user") ));
	
	hide();
}


KHLogin::~KHLogin()
{
	delete m_selector;
}

void KHLogin::setPlayerInformation()
{
	KHWarrior *wtmp = m_selector->currentWarrior();
	
	PLAYER->setWarriorInformation(wtmp->face(), wtmp->login(), wtmp->theName(), "Human" );
	PLAYER->setElo(wtmp->elo());
	
	m_labelName->setText( i18n("Name: ")+PLAYER->theName() );
	m_labelElo->setText( i18n("Elo: ")+QString::number(PLAYER->elo()) );
	
	QString type = "";
	switch(PLAYER->type())
	{
		case KHWarriorBase::Human:
		{
			type = i18n("Human");
		}
		break;
		case KHWarriorBase::Engine:
		{
			type = i18n("Engine");
		}
		break;
		case KHWarriorBase::Internet:
		{
			type = i18n("Internet");
		}
		break;
	}
	
	m_labelType->setText(i18n("Type: ")+type);
	
	m_login->setText(PLAYER->login());
	
}

void KHLogin::slotOk()
{
	if ( ! m_login->text().isEmpty() )
	{
		KDialogBase::slotOk();
	}
	else
	{
		KMessageBox::error(this, i18n("Please select a player"), i18n("Error"));
	}
}

void KHLogin::slotUser1()
{
	done( 123 );
}


#include "khlogin.moc"
