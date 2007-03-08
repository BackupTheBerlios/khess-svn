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

#include "engineinterface.h"

#include <QProcess>

#include <QtDebug>

namespace IO {

struct EngineInterface::Private
{
	QProcess *engine;
};

EngineInterface::EngineInterface(QObject *parent) : IO::Interface(parent), d( new Private)
{
	d->engine = 0;
}

EngineInterface::~EngineInterface()
{
	closeResource();
	delete d;
}

void EngineInterface::doMove(const QString &san)
{
	qDebug() << "MOVE: " << san;
	send(san);
}

bool EngineInterface::openResource(const InterfaceParams *params)
{
	d->engine = new QProcess(this);
	
	connect(d->engine, SIGNAL(readyReadStandardOutput()), this, SLOT(parseData()));
	connect(d->engine, SIGNAL(readyReadStandardError()), this, SLOT(parseError()));
	
	d->engine->start(params->node(), params->args());
	bool ok = d->engine->waitForStarted();
	
	if ( ok )
	{
		send("xboard");
		send("protover 2");
		send("hard");
	}
	
	return ok;
}

bool EngineInterface::closeResource()
{
	if ( d->engine )
	{
		send("quit");
		d->engine->terminate();
		d->engine->waitForFinished();
		
		if ( d->engine->isOpen() )
		{
			d->engine->kill();
		}
		
		return d->engine->isOpen();
	}
	
	return false;
}

void EngineInterface::send(const QString &data)
{
	d->engine->write((data+"\n").toLocal8Bit());
}

void EngineInterface::parseData()
{
	QString data = QString::fromLocal8Bit(d->engine->readAllStandardOutput());
	
	qDebug() << "DATA: " << data;
	
	QStringList fields = data.split('\n');
	
	QRegExp moveRX("^move (.*)");
	
	foreach(QString line, fields)
	{
		if ( line.length() < 3 )
		{
			continue;
		}
		else if ( moveRX.indexIn(line) != -1)
		{
			QString move = moveRX.cap(1);
			emit moved(move);
		}
	}
}

void EngineInterface::parseError()
{
	QString data = QString::fromLocal8Bit(d->engine->readAllStandardError());
	
	qDebug() << "ERROR: " << data;
}

}
