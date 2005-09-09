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
#ifndef KHINTERNETIOINTERFACE_H
#define KHINTERNETIOINTERFACE_H

#include <khiointerface.h>
#include <kstreamsocket.h>
#include <qsocket.h>

class ConnectionParam;

using KNetwork::KStreamSocket;
using KNetwork::KClientSocketBase;

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHInternetIOInterface : public KHIOInterface
{
	Q_OBJECT
	public:
		KHInternetIOInterface();
		~KHInternetIOInterface();
		void sendToResource(const QByteArray &buffer);
		bool isConnected();
		
	public slots:
		void connectToResource(const QString &server, const QString &port);
		void closeResource();
		void sendToResource( const QString &data);
		
		
	private slots:
		void socketConnected();
		void socketError(int);
		void socketReadyToRead();
		void verifyConnection(int s);
		
	private:
		KStreamSocket *m_socket;
};

class ConnectionParam
{
	public:
		ConnectionParam(const QString &server, int port, const QString &login, const QString &password);
		~ConnectionParam();
		
		void setGuest(bool p);
		bool guest() const;
		QString server() const;
		int port() const;
		QString login() const;
		QString password() const;
		
	private:
		QString m_server;
		int m_port;
		QString m_login;
		QString m_password;
		
		bool m_guest;
};


#endif
