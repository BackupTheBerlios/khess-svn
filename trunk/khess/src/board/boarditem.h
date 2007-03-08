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
#ifndef BOARDITEM_H
#define BOARDITEM_H

#include <QGraphicsItem>

#include <game/common.h>
#include <game/move.h>

namespace Game {
	class Game;
}

namespace Board {

class PieceItem;

/**
 * @author Jorge Cuadrado <kuadrosx@zi0n>
*/

class BoardItem : public QObject, public QGraphicsItem
{
	Q_OBJECT;
	
	public:
		BoardItem(QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		~BoardItem();
		void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
		QRectF boundingRect () const;
		
		void setGame(Game::Game *const game);
		
		void movePiece(PieceItem* piece, const Game::Square &from, const Game::Square &to);
		void setPiece(PieceItem* piece, int row, int col );
		void setPiece(PieceItem *piece, const Game::Square &square);
		
		bool reposition(PieceItem* piece);
		
		void drawBoard();
		
	protected:
		QPoint toCoord(const Game::Square &square);
		Game::Square toSquare(const QPoint &coord);
		Game::Square toSquare(int row, int col);
		
	private:
		void doMove(PieceItem* piece, const Game::Move &move);
		
	public slots:
		void doMove(const QString &san);
		
	signals:
		void moved(const QString &san);
		
	private:
		struct Private;
		Private * const d;
};

}

#endif
