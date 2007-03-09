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

#include "internetinterface.h"

#include <QTcpSocket>
#include <QtDebug>

namespace IO {

struct InterfaceNetParams::Private
{
	QString login;
	QString password;
	bool logAsGuest;
};

InterfaceNetParams::InterfaceNetParams() : d(new Private)
{
	d->logAsGuest = false;
}

InterfaceNetParams::~InterfaceNetParams()
{
	delete d;
}

void InterfaceNetParams::setLogin(const QString &login)
{
	d->login = login;
}

void InterfaceNetParams::setPassword(const QString &password)
{
	d->password = password;
}

void InterfaceNetParams::setLogAsGuest(bool e)
{
	d->logAsGuest = e;
}

QString InterfaceNetParams::password() const
{
	return d->password;
}

QString InterfaceNetParams::login() const
{
	return d->login;
}

bool InterfaceNetParams::logAsGuest() const
{
	return d->logAsGuest;
}

// IFACE

struct InternetInterface::Private
{
	QTcpSocket *socket;
	
	QString login;
	QString password;
	bool logAsGuest;
	
	bool isLogged;
};

InternetInterface::InternetInterface(QObject *parent)
 : IO::Interface(parent), d(new Private)
{
	d->socket = new QTcpSocket(this);
	
	connect(d->socket, SIGNAL(readyRead()), this, SLOT(parseData()));
	connect(d->socket, SIGNAL(disconnected()), this, SLOT(handleDisconnection()));
}


InternetInterface::~InternetInterface()
{
	delete d;
}

void InternetInterface::doMove(const QString &san)
{
}

bool InternetInterface::openResource(const InterfaceParams *params)
{
	const InterfaceNetParams *inetparams = static_cast<const InterfaceNetParams *>(params);
	
	int port = 5000;
	if ( !params->args().isEmpty() )
	{
		port = params->args().first().toInt();
	}
	
	qDebug() << "Connecting to " << params->node() << ":" << port << " " << inetparams->login();
	
	d->socket->connectToHost(params->node(), port);
	
	bool ok = d->socket->waitForConnected();
	
	if( ok )
	{
		d->login = inetparams->login();
		d->password = inetparams->password();
		d->logAsGuest = inetparams->logAsGuest();
	}
	
	d->isLogged = false;
	
	return ok;
}

void InternetInterface::send(const QString &data)
{
	d->socket->write((data+"\n").toLocal8Bit());
}

bool InternetInterface::closeResource()
{
	d->socket->disconnectFromHost();
	
	bool ok = d->socket->waitForDisconnected();
	
	return ok;
}


void InternetInterface::parseData()
{
	QString data = QString::fromLocal8Bit(d->socket->readAll());
	
	QStringList fields = data.split('\n');
	
	foreach(QString line, fields)
	{
		line = line.trimmed();
		
		qDebug() << "LINE: " << line;
		
		if( ! d->isLogged )
		{
			if( line.endsWith("login:") )
			{
				send(d->login);
			}
			else if ( line.endsWith("password:") )
			{
				send(d->password);
				d->isLogged = true;
			}
			else if( line.contains("Press return to enter the server as") )
			{
				send("");
				d->isLogged = true;
			}
			
			if( d->isLogged )
			{
				send("set style 12");
				send("set interface Khess 0.1alpha");
			}
		}
		else if ( line.endsWith("fics%") ) // Commands
		{
			
		}
		else // Responses
		{
		}
	}
}

void InternetInterface::handleDisconnection()
{
	qWarning("DISCONNECTED!!!");
}

}
