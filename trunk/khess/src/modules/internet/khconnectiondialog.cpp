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

#include "khconnectiondialog.h"

#include <klocale.h>
#include <kdebug.h>
#include <qvbox.h>
#include <kseparator.h>
#include <kiconloader.h>

KHConnectionDialog::KHConnectionDialog(QWidget *parent) : KDialogBase(parent, "KHConnectionDialog", true, i18n("Connect to server..."), User1 | Ok | Cancel, Ok,true)
{
	QVBox *page = makeVBoxMainWidget();

	QVGroupBox *serverInfo = new QVGroupBox(i18n("Server information"), page);
	
	new QLabel(i18n("Server"), serverInfo);
	m_serverLE = new KLineEdit(serverInfo);
	m_serverLE->setText("freechess.org");
	
	new QLabel(i18n("Port"), serverInfo);
	m_portLE = new KLineEdit(serverInfo);
	m_portLE->setText("5000");
	
	QVGroupBox *userInfo = new QVGroupBox(i18n("User information"), page);
	
	new QLabel(i18n("User"), userInfo);
	m_loginLE = new KLineEdit(userInfo);
	
	new QLabel(i18n("Password"), userInfo);
	m_passwordLE= new KLineEdit(userInfo);
	m_passwordLE->setEchoMode( KLineEdit::Password);
	
	m_guest = new QCheckBox(i18n("Loggin as guest"), userInfo);
	connect(m_guest, SIGNAL(stateChanged(int)), this, SLOT(verifyGuest(int)));
	
	setButtonGuiItem (KDialogBase::User1, KGuiItem( i18n("Register"), SmallIcon("network"), i18n("Click for register"), i18n("Clicking this button you can register in FICS") ));
}


KHConnectionDialog::~KHConnectionDialog()
{
}

void KHConnectionDialog::slotOk()
{
	// Validar datos!
	
	KDialogBase::slotOk();
}

void KHConnectionDialog::slotUser1()
{
	done( Register );
}

void KHConnectionDialog::verifyGuest(int opt)
{
	if (opt == QCheckBox::On )
	{
		m_login = m_loginLE->text();
		m_loginLE->setText("guest");
		m_loginLE->setReadOnly(true);
		m_passwordLE->setReadOnly(true);
	}
	else
	{
		m_loginLE->setText(m_login);
		m_loginLE->setReadOnly(false);
		m_passwordLE->setReadOnly(false);
	}
}

QString KHConnectionDialog::login()
{
	m_login = m_loginLE->text();

	return m_login;
}

QString KHConnectionDialog::password()
{
	return m_passwordLE->text();
}

QString KHConnectionDialog::server()
{
	m_login = m_serverLE->text();

	return m_login;
}

QString KHConnectionDialog::port()
{
	return m_portLE->text();
}

bool KHConnectionDialog::asGuest()
{
	return m_guest->isChecked();
}



#include "khconnectiondialog.moc"
