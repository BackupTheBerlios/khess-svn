/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado   *
 *   kuadrosx@zi0n   *
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
#ifndef PIECEITEM_H
#define PIECEITEM_H

#include <QGraphicsSvgItem>

#include <game/common.h>

namespace Board {

class BoardItem;

/**
 * @author Jorge Cuadrado <kuadrosx@zi0n>
*/
class PieceItem : public QGraphicsSvgItem
{
	public:
		PieceItem(BoardItem *board, const Game::Piece &type);
		~PieceItem();
		
	protected:
		void chooseSvg(const Game::Piece &type);
		void mousePressEvent ( QGraphicsSceneMouseEvent * event );
		void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
		QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
		
	private:
		struct Private;
		Private * const d;
};

}

#endif
