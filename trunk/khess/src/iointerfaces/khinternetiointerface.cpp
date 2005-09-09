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

#include "khinternetiointerface.h"

#include <kdebug.h>
#include <klocale.h>
#include <kdeversion.h>
#include <qregexp.h>

using namespace KNetwork;

KHInternetIOInterface::KHInternetIOInterface() : KHIOInterface(), m_socket(0)
{
}


KHInternetIOInterface::~KHInternetIOInterface()
{
	kdDebug() << "[Destroying KHInternetIOInterface]" << endl;
	if ( m_socket )
	{
		if(m_socket->state() ==  KClientSocketBase::Open )
		{
			m_socket->close();
		}
	}
}

void KHInternetIOInterface::connectToResource(const QString &server, const QString &port)
{
	kdDebug() << "Connecting to " << server << endl;
	m_socket = new KStreamSocket(server, port );

// 	m_socket->setTimeout(1000);
	
	connect(m_socket, SIGNAL(connected(const KResolverEntry&)),this, SLOT(socketConnected()));
	connect(m_socket, SIGNAL(gotError(int)), this, SLOT(socketError(int)));
	connect(m_socket, SIGNAL(readyRead()),this, SLOT(socketReadyToRead()));
	connect(m_socket, SIGNAL(stateChanged(int)), this, SLOT(verifyConnection(int)));
	
	m_socket->enableRead(true);
	
	if( ! m_socket->connect() )
	{
		kdError() << m_socket->errorString() << endl;
	}
}

void KHInternetIOInterface::closeResource()
{
	m_socket->close();
}

void KHInternetIOInterface::socketConnected()
{
	kdDebug() << "Connected!" << endl;
}

void KHInternetIOInterface::socketReadyToRead()
{
	QString leido = "";
	int bytes = m_socket->bytesAvailable();
	
	if ( bytes > 0)
	{
		QByteArray buffer(bytes);
		
		m_socket->readBlock(buffer.data(), bytes);
		QString data(buffer.data());
		
		data.remove(QRegExp( "f?ics%.*"));
		data.remove(QRegExp("\\x0007"));
		data.remove( QRegExp("[\\t\\b\\f^(\\\)]") );//\\n|
		
		kdDebug() << "[-" << data << "-]" << endl;
		
		emit dataReaded(data);
	}
}

void KHInternetIOInterface::sendToResource(const QByteArray &buffer)
{
	m_socket->writeBlock(buffer, buffer.count());
}

void KHInternetIOInterface::sendToResource(const QString &data)
{
	kdDebug() << "LLEGA: " << data << endl;
	QTextStream os(m_socket);
	QString tmp(data);
	if( tmp.at(tmp.length()) != '\n' )
	{
		tmp.append('\n');
	}
	kdDebug() << "Escribiendo al socket: " << tmp << endl;
	
	os << tmp;
}

void KHInternetIOInterface::verifyConnection(int s)
{
	if ( s == KClientSocketBase::Unconnected )
	{
		kdDebug() << "Unconnected" << endl;
	}
	else if ( s == KClientSocketBase::Closing )
	{
		kdDebug() << "Closing" << endl;
	}
	else
	{
// 		kdDebug() << "State changed to " << s << endl;
	}
}

void KHInternetIOInterface::socketError(int e)
{
	kdError() << "Error " << e << endl;
	switch(e)
	{
		case KSocketBase::NoError:
		{
			kdError() <<   "no error has been detected"      << endl;
		}
		break;
		case KSocketBase::LookupFailure:
		{
			kdError() <<    "if a name lookup has failed"     << endl;
		}
		break;
		case KSocketBase::AddressInUse:
		{
			kdError() <<    "address is already in use"     << endl;
		}
		break;
		case KSocketBase::AlreadyBound:
		{
			kdError() <<     "cannot bind again"    << endl;
			
		}
		break;
		case KSocketBase::AlreadyCreated:
		{
			kdError() <<    "cannot recreate the socket"     << endl;
		}
		break;
		case KSocketBase::NotBound:
		{
			kdError() <<    "operation required socket to be bound and it isn't"      << endl;
		}
		break;
		case KSocketBase::NotCreated:
		{
			kdError() <<    "operation required socket to exist and it doesn't"    << endl;
		}
		break;
		case KSocketBase::WouldBlock:
		{
			kdError() <<   "requested I/O operation would block"      << endl;
		}
		break;
		case KSocketBase::ConnectionRefused:
		{
			kdError() <<     "connection actively refused"     << endl;
		}
		break;
		case KSocketBase::ConnectionTimedOut:
		{
			kdError() <<    "connection timed out"      << endl;
		}
		break;
		case KSocketBase::InProgress:
		{
			kdError() <<    "operation (connection) is already in progress"      << endl;
		}
		break;
		case KSocketBase::NetFailure:
		{
			kdError() <<    "a network failure occurred (no route, host down, host unreachable or similar)"      << endl;
		}
		break;
		case KSocketBase::NotSupported:
		{
			kdError() <<   "requested operation is not supported"      << endl;
		}
		break;
		case KSocketBase::Timeout:
		{
			kdError() <<    "a timed operation timed out"     << endl;
		}
		break;
		case KSocketBase::UnknownError:
		{
			kdError() <<    "an unknown/unexpected error has happened"     << endl;
		}
		break;
#if KDE_IS_VERSION(3,4,0)
		case KSocketBase::RemotelyDisconnected:
{
	kdError() << "when a connection is disconnected by the other end"     << endl;
}
		break;
#endif
	}
}

bool KHInternetIOInterface::isConnected()
{
	if ( m_socket->state() == KClientSocketBase::Connected )
	{
		return true;
	}
	
	return false;
}

ConnectionParam::ConnectionParam(const QString &server, int port, const QString &login, const QString &password) : m_server(server), m_port(port), m_login(login), m_password(password), m_guest(false)
{
}

ConnectionParam::~ConnectionParam()
{
}

void ConnectionParam::setGuest(bool p)
{
	m_guest = p;
}

bool ConnectionParam::guest() const
{
	return m_guest;
}

QString ConnectionParam::server() const
{
	return m_server;
}

int ConnectionParam::port() const
{
	return m_port;
}

QString ConnectionParam::login() const
{
	return m_login;
}

QString ConnectionParam::password() const
{
	return m_password;
}

#include "khinternetiointerface.moc"
