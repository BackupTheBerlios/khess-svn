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

#include "matchdialog.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QtDebug>

#include <QProcess>

#include <interfaces/internetinterface.h>

#include <dformfactory.h>

struct MatchDialog::Private
{
	IO::InterfaceParams *params;
	
	QComboBox *engine;
};

MatchDialog::MatchDialog()
	: DTabDialog(DTabDialog::Ok|DTabDialog::Cancel), d(new Private)
{
	QWidget *page1 = new QWidget;
	
	QVBoxLayout *layout = new QVBoxLayout(page1);
	
	d->engine = new QComboBox;
	findEngines();
	
	layout->addWidget(d->engine);
	
	addTab(page1, tr("Configure"));
	
	d->params = new IO::InterfaceParams;
}


MatchDialog::~MatchDialog()
{
	delete d;
}

void MatchDialog::setupInetBox()
{

}

void MatchDialog::findEngines()
{
	QStringList engines = QStringList() << "crafty" << "gnuchess";
	
	foreach(QString engine, engines)
	{
		QProcess proc;
		proc.start(engine);
		
		if( proc.waitForStarted() )
		{
			d->engine->addItem(engine);
			
			proc.terminate();
			proc.waitForFinished();
			proc.kill();
		}
	}
}

IO::InterfaceParams *MatchDialog::params()
{
	QString engine = d->engine->currentText();
	
	if(engine.isEmpty()) return 0;
	
	d->params->setNode(engine);
	
	return d->params;
}




