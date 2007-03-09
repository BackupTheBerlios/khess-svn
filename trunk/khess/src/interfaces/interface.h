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

#ifndef IOINTERFACE_H
#define IOINTERFACE_H

#include <QObject>
#include <QStringList>

namespace IO {

class InterfaceParams
{
	public:
		InterfaceParams();
		~InterfaceParams();
		
		void setNode(const QString &node);
		void setArgs(const QStringList &args);
		
		QString node() const;
		QStringList args() const;
		
	private:
		struct Private;
		Private *const d;
};

/**
 * Interfaz con motores o internet.
 * @author David Cuadrado <krawek@gmail.com >
*/
class Interface : public QObject
{
	Q_OBJECT
	public:
		Interface(QObject *parent = 0);
		virtual ~Interface();
		
		virtual void send(const QString &data) = 0;
		
	public slots:
		virtual void doMove(const QString &san) = 0;
		virtual bool openResource(const InterfaceParams *params ) = 0;
		virtual bool closeResource() = 0;
		
	signals:
		void moved(const QString &fen);
		
};

}

#endif
