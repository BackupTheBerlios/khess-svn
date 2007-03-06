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

#include "khmatchdialog.h"

#include <qvbox.h>
#include <qhbox.h>
#include <qlayout.h>
#include <kdebug.h>
#include <klocale.h>
#include <kseparator.h>

#include <qlabel.h>
#include <qapplication.h>
#include <qpalette.h>

KHMatchDialog::KHMatchDialog(QWidget *parent) : KDialogBase(parent, "KHMatchDialog", true, i18n("Create a new match"), Ok | Cancel, Ok, true), m_match(0)
{
	QVBox *page = makeVBoxMainWidget();
	page->setMargin(5);
	page->setLineWidth(1);
	page->setMidLineWidth(1);
	page->setFrameStyle (QFrame::Box | QFrame::Raised );

	new QLabel(i18n("Select your oponent"), page);
	
	QVBox *opponentInfo = new QVBox(page);

	m_oppName = new QLabel(i18n("Name"), opponentInfo);
	m_oppElo = new QLabel(i18n("ELO"), opponentInfo);
	m_oppType = new QLabel(i18n("Type"), opponentInfo);
	
	m_userSelector = new KHSelectUser(page);
	
	connect(m_userSelector, SIGNAL(playerSelected()), SLOT(readOpponentInformation()));
	
	QHBox *box = new QHBox(page);
	m_color = new QCheckBox(box);
	m_color->setText(i18n("I want to play with white"));
	m_color->setChecked(true);
	
	m_rated = new QCheckBox(box);
	m_rated->setText(i18n("rated"));
	
	new KSeparator(page);
	
	m_time = new KIntNumInput(page);
	m_time->setLabel(i18n("Time"), AlignTop);
	m_time->setRange (0, 120);
	
	m_increment = new KIntNumInput(page);
	m_increment->setLabel(i18n("Increment"), AlignTop);
	m_increment->setRange (0, 15);
}


KHMatchDialog::~KHMatchDialog()
{
}

bool KHMatchDialog::isValid()
{
	bool ok = true;
	
	if(  m_time->value() == 0 )
	{
		ok = ok && false;
		m_time->setPaletteBackgroundColor( Qt::red );
		QTimer::singleShot(3000, this, SLOT(restore()));
	}
	
	return ok;
}

void KHMatchDialog::slotOk()
{
	KHWarrior * opponent = m_userSelector->currentWarrior();
	
	if ( ! opponent || !isValid() || opponent->login() == "...")
	{
		return;
	}
	
	KHMatch::Type matchType;
	
	if ( opponent->type() == KHWarrior::Human)
	{
		matchType = KHMatch::LOCAL;
	}
	else if ( opponent->type() == KHWarrior::Engine)
	{
		matchType = KHMatch::ENGINE;
	}

	QString id = "";
	if ( m_color->isChecked() )
	{
		id = PLAYER->login()+"_vs_"+opponent->login()+"-"+QDate::currentDate().toString( Qt::ISODate);
		m_match = new KHMatch(matchType, PLAYER, opponent, id, m_rated->isChecked());
	}
	else
	{
		id = opponent->login()+"_vs_"+PLAYER->login()+"-"+QDate::currentDate().toString( Qt::ISODate);
		m_match = new KHMatch(matchType, opponent, PLAYER, id, m_rated->isChecked());
	}
	
	m_match->setTime( m_time->value()*60, m_increment->value()*60 );
	
	KDialogBase::slotOk();
}

KHMatch *KHMatchDialog::match()
{
	Q_CHECK_PTR(m_match);
	return m_match;
}

void KHMatchDialog::restore()
{
	m_time->setPaletteBackgroundColor( QApplication::palette(this).color(QPalette::Normal, QColorGroup::Background) );
}

void KHMatchDialog::readOpponentInformation()
{
	KHWarrior *wtmp = m_userSelector->currentWarrior();
	
	m_oppName->setText( i18n("Name: ")+wtmp->theName() );
	m_oppElo->setText( i18n("ELO: ")+QString::number(wtmp->elo()) );
	
	QString type = "";
	switch(wtmp->type())
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
	
	m_oppType->setText(i18n("Type: ")+type);
}

#include "khmatchdialog.moc"
