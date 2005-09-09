/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "khimagemanager.h"
#include <kdebug.h>

KHImageManager::KHImageManager()
{
	kdDebug() << "[Initializing KHImageManager]" << endl;
	imageVector.resize(Background);
}


KHImageManager::~KHImageManager()
{
	kdDebug() << "[Destroying KHImageManager]" << endl;
	imageVector.clear();
}

bool KHImageManager::add(Images index, const QImage &img)
{
	if ( (uint) index > imageVector.count() )
	{
		kdError() << "KHImageManager::add: Out of index" << endl;
		return false;
	}

	imageVector[index] = img;
	
	return true;
}

QImage KHImageManager::get(Images index, int size)
{
	QImage imgTmp = imageVector[index];
	
	if ( imgTmp.isNull() )
	{
		kdDebug() << "Image NULL" << endl;
		return imgTmp;
	}
	
	int width = 0, height = 0;
	
	switch(index)
	{
		case Border:
		{
			width = size * 9;
			height = size * 9;
		}
		break;
		case BorderLightOn:
		{
			width = 0;
			height = 0;
		}
		break;
		case BorderLightOff:
		{
			width = 0;
			height = 0;
		}
		break;
		case BorderLightOn2:
		{
			width = 0;
			height = 0;
		}
		break;
		case BorderLightOff2:
		{
			width = 0;
			height = 0;
		}
		break;
		case SquareWhite:
		{
			width = size;
			height = size;
		}
		break;
		case SquareDark:
		{
			width = size;
			height = size;
		}
		break;
		case Background:
		{
			width = 0;
			height = 0;
		}
		break;
		default:
		{
			width = size;
			height = size;
		}
		break;
	}
		
	return imgTmp.smoothScale(width, height);
}

QImage KHImageManager::getWithFade(Images index, int size, float val, const QColor &color )
{
	QImage imgTmp = get(index,size);
	return KImageEffect::fade( imgTmp, val, color);
}
