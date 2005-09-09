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

#include "khselectfacebutton.h"
#include <klocale.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <qimage.h>
#include <kpixmap.h>

#include <kfiledialog.h>


KHSelectFaceButton::KHSelectFaceButton(QWidget *parent, const char *name) : KPushButton(parent, name)
{
	QPixmap pixdefault = QPixmap( ::locate("data", "khess/faces/default3.png"));
	
	setPixmap( QPixmap( ::locate("data", "khess/faces/default3.png")) );
	setup();
}

KHSelectFaceButton::KHSelectFaceButton(const QPixmap &defaultPixmap, QWidget *parent, const char *name) : KPushButton(parent, name)
{
	setPixmap( defaultPixmap );
	setup();
}


KHSelectFaceButton::~KHSelectFaceButton()
{
}

void KHSelectFaceButton::setup()
{
	setMaximumSize(MAX_IMAGE_SIZE, MAX_IMAGE_SIZE);
	
	connect(this, SIGNAL(clicked()), this, SLOT(selectAFace()));
}

void KHSelectFaceButton::selectAFace()
{
	QString imagePath = KFileDialog::getImageOpenURL ( QDir::homeDirPath(), this, i18n("Select a face")).path();
	
	if ( ! imagePath.isEmpty() )
	{
		KPixmap image(imagePath);
		setPixmap(image);
	}
}

void KHSelectFaceButton::setPixmap ( const QPixmap & pix )
{
	QImage imgTmp(pix.convertToImage () );
	QPixmap newPixmap(imgTmp.smoothScale(MAX_IMAGE_SIZE,MAX_IMAGE_SIZE));
	
	KPushButton::setPixmap(newPixmap);
}

#include "khselectfacebutton.moc"
