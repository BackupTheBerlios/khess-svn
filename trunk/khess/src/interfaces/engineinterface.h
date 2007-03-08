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

#ifndef IOENGINEINTERFACE_H
#define IOENGINEINTERFACE_H

#include <interfaces/interface.h>

namespace IO {

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class EngineInterface : public IO::Interface
{
	Q_OBJECT
	public:
		EngineInterface(QObject *parent = 0);
		~EngineInterface();
		
	public slots:
		void doMove(const QString &san);
		bool openResource(const QString &command, const QStringList &args = QStringList());
		void send(const QString &data);
		bool closeResource();
		
	protected slots:
		void parseData();
		void parseError();
		
	signals:
		void moved(const QString &fen);
		
	private:
		struct Private;
		Private *const d;
};

}

#endif
