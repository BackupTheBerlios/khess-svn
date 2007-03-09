/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#include "connectdialog.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>

#include <dformfactory.h>

#include <interfaces/internetinterface.h>

struct ConnectDialog::Private
{
	IO::InterfaceNetParams *params;
	QLineEdit *login, *password, *server;
	QSpinBox *port;
	QCheckBox *asGuest;
};

ConnectDialog::ConnectDialog(QWidget *parent)
 : QDialog(parent), d(new Private)
{
	d->params = new IO::InterfaceNetParams;
	
	QVBoxLayout *layout = new QVBoxLayout;
	
	d->login = new QLineEdit;
	d->password = new QLineEdit;
	d->password->setEchoMode(QLineEdit::Password);
	
	d->server = new QLineEdit;
	d->server->setText("freechess.org");
	
	d->port = new QSpinBox;
	d->port->setMinimum(1);
	d->port->setMaximum(65000);
	d->port->setValue(5000);
	
	layout->addLayout(DFormFactory::makeGrid(QStringList() << "Login" << "Password" << "Server" << "Port", QWidgetList() << d->login << d->password << d->server << d->port ));
	
	d->asGuest = new QCheckBox(tr("Login as guest"));
	
	layout->addWidget(d->asGuest);
	
	
	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
	connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
	
	layout->addWidget(buttons);
	
	setLayout(layout);
}


ConnectDialog::~ConnectDialog()
{
	delete d;
}

IO::InterfaceNetParams *ConnectDialog::params()
{
	d->params->setNode(d->server->text());
	d->params->setArgs(QStringList() << QString::number(d->port->value()) );
	
	d->params->setLogin(d->login->text());
	d->params->setPassword(d->password->text());
	d->params->setLogAsGuest(d->asGuest->isChecked());
	
	
	return d->params;
}


