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

#ifndef IOINTERNETINTERFACE_H
#define IOINTERNETINTERFACE_H

#include <interfaces/interface.h>

namespace IO {

class InterfaceNetParams :  public IO::InterfaceParams
{
	public:
		InterfaceNetParams();
		~InterfaceNetParams();
		void setLogin(const QString &login);
		void setPassword(const QString &password);
		void setLogAsGuest(bool e);
		
		QString password() const;
		QString login() const;
		bool logAsGuest() const;
	
	
	private:
		struct Private;
		Private *const d;
};

/**
	@author David Cuadrado <krawek@gmail.com>
*/
class InternetInterface : public IO::Interface
{
	Q_OBJECT
	public:
		InternetInterface(QObject *parent = 0);
		~InternetInterface();
		
	public slots:
		void doMove(const QString &san);
		bool openResource(const InterfaceParams *params);
		void send(const QString &data);
		bool closeResource();
		
	protected slots:
		void handleDisconnection();
		void parseData();
		
	private:
		struct Private;
		Private *const d;
};

}

#endif
