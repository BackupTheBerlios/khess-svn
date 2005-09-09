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

#ifndef KHIMAGEMANAGER_H
#define KHIMAGEMANAGER_H

#include <qobject.h>
#include <qimage.h>
#include <qvaluevector.h>

#include <kimageeffect.h>

/**
	@author David Cuadrado - Jorge Cuadrado <krawek@gmail.com - kuadrosxx@gmail.com>
*/

class KHImageManager : public QObject
{
	public:
		enum Images
		{
			Border = 0,
			BorderLightOn,
			BorderLightOff,
			BorderLightOn2,
			BorderLightOff2,
			SquareWhite,
			SquareDark,
			BlackKing,
			BlackQueen,
			BlackBishop,
			BlackKnight,
			BlackRook,
			BlackPawn,
			WhiteKing,
			WhiteQueen,
			WhiteBishop,
			WhiteKnight,
			WhiteRook,
			WhitePawn,
			Select,
			Motion,
			Danger,
			Background
		};
		
		KHImageManager();
		~KHImageManager();
		
		bool add(Images index, const QImage &img);
		QImage get(Images index, int size );
		QImage getWithFade(Images index, int size, float val, const QColor &color );
		
	private:
		QValueVector<QImage> imageVector;
		
		
};

#endif
