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
#include "boardview.h"

#include <QGraphicsScene>

#include "boarditem.h"
#include "pieceitem.h"

#include "game/game.h"

namespace Board {

struct BoardView::Private
{
	QList<BoardItem *> boards;
};

BoardView::BoardView(QWidget * parent) : QGraphicsView(parent), d(new Private)
{
	setAlignment(Qt::AlignCenter);
	
	QGraphicsScene *scene = new QGraphicsScene();
	setScene(scene);
	
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
	
#if 0
	PieceItem *pawn = new PieceItem(d->boardItem, 'P');
	d->boardItem->setPiece(pawn, 0,0);
	
	PieceItem *pawn1 = new PieceItem(d->boardItem, 'p');
	d->boardItem->setPiece(pawn1, 0,1);
	
	scene->addItem(pawn);
	scene->addItem(pawn1);
#endif 
}

BoardView::~BoardView()
{
	delete d;
}

QSize BoardView::sizeHint() const
{
	return QSize(400,400); // FIXME
}

Board::BoardItem *BoardView::createBoard(Game::Game *const game)
{
	BoardItem *board = new BoardItem();
	scene()->addItem(board);
	
	centerOn(board->boundingRect().center());
	
	board->setGame(game);
	
	d->boards << board;
	
	update();
	
	return board;
}


void BoardView::drawBackground(QPainter *painter, const QRectF &rect)
{
	painter->fillRect(rect, Qt::gray);
}

}


