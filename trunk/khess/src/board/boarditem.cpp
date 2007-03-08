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

#include "boarditem.h"

#include <QPainter>
#include <QVector>
#include <QGraphicsScene>

#include <QDebug>

#include "pieceitem.h"

#include "game/game.h"

namespace Board {

class SquareItem : public QGraphicsItem
{
	public:
		enum { Type = UserType + 1 };
		SquareItem(QGraphicsItem *board, const Game::Square &logicalIndex, bool white);
		~SquareItem();
		
		
		void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
		
		QRectF boundingRect() const;
		
		int type() const
		{
			return Type;
		};
		
		void setCurrentPiece(Board::PieceItem *currentPiece);
		void replaceCurrentPiece(Board::PieceItem *currentPiece);
		Board::PieceItem *currentPiece() const;
		
		Game::Square logicalIndex() const;
		
	private:
		bool m_white;
		Game::Square m_logicalIndex;
		
		Board::PieceItem *m_currentPiece;
};

SquareItem::SquareItem(QGraphicsItem *board, const Game::Square &logicalIndex, bool white) : QGraphicsItem(board), m_white(white), m_logicalIndex(logicalIndex), m_currentPiece(0)
{
	
}

SquareItem::~SquareItem()
{
}

void SquareItem::setCurrentPiece(Board::PieceItem *currentPiece)
{
	m_currentPiece = currentPiece;
}

void SquareItem::replaceCurrentPiece(Board::PieceItem *currentPiece)
{
	if( m_currentPiece )
	{
		delete m_currentPiece;
		m_currentPiece = 0;
	}
	
	m_currentPiece = currentPiece;
}

Board::PieceItem *SquareItem::currentPiece() const
{
	return m_currentPiece;
}

Game::Square SquareItem::logicalIndex() const
{
	return m_logicalIndex;
}

void SquareItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QColor c;
	if(m_white)
	{
		c = QColor( 0xAEB2C3 );
	}
	else
	{
		c = QColor( 0xc37254 );
	}
	
	painter->fillRect(boundingRect(), c);
	
}

QRectF SquareItem::boundingRect () const
{
	return QRectF(0,0,32,32);
}

struct BoardItem::Private
{
	QVector< SquareItem *> squares;
	Game::Game *game;
	
};

BoardItem::BoardItem( QGraphicsItem * parent , QGraphicsScene * scene ) : QGraphicsItem(), d( new Private)
{
	d->squares.resize(64);
	for(Game::Square sq = 0; sq < 64; sq++)
	{
		QPoint coord = toCoord(sq);
		
		bool white = (coord.x()+coord.y()) % 2 == 0;
		
		SquareItem *square = new SquareItem( this, sq, white);
		
		square->setPos(coord.x() * 32, coord.y() * 32 );
		
		d->squares[sq] = square;
	}
}


BoardItem::~BoardItem()
{
}

void BoardItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	//TODO: dibujar borde
	painter->setPen(QPen( QColor(0x91642e), 15));
	painter->drawRoundRect(boundingRect(), 5, 5 );
}

QRectF BoardItem::boundingRect () const
{
	return QRectF(0,0, 32*8, 32*8);
}

void BoardItem::setGame(Game::Game *const game)
{
	d->game = game;
	drawBoard();
}

void BoardItem::drawBoard()
{
	Game::Board board = d->game->board();
	
	for(Game::Square square = 0; square < 64; square++)
	{
		d->squares[square]->replaceCurrentPiece(0); // Remove all pieces
		
		Game::Piece pieceType = board.at(square);
		if(pieceType != 0 && pieceType != 255)
		{
			PieceItem *piece = new PieceItem(this, pieceType);
			piece->setCurrentSquare(square);
			
			scene()->addItem(piece);
			
			setPiece(piece, square);
		}
	}
}

void BoardItem::movePiece(PieceItem* piece, const Game::Square &from, const Game::Square &to)
{
	setPiece(piece, to );
	d->squares[from]->setCurrentPiece(0);
}

void BoardItem::setPiece(PieceItem * piece, int row, int col )
{
	setPiece(piece, toSquare(row, col));
}

void BoardItem::setPiece(PieceItem *piece, const Game::Square &square)
{
	piece->setParentItem(this);
	
	Board::SquareItem *squareItem = d->squares[square];
	
	squareItem->setZValue(1);
	piece->setPos(squareItem->pos());
	
	squareItem->replaceCurrentPiece(piece);
	
	piece->setCurrentSquare(square);
}

QPoint BoardItem::toCoord(const Game::Square &square)
{
	// TODO: flip
	return QPoint(square % 8, 7 - square / 8);
}

Game::Square BoardItem::toSquare(const QPoint &coord)
{
	// TODO: flip
	return toSquare(coord.x(), coord.y());
}

Game::Square BoardItem::toSquare(int row, int col)
{
	return 8 * row + col;
}

bool BoardItem::reposition(PieceItem* piece)
{
	QList<QGraphicsItem *> items = piece->collidingItems();
	
	QMap<double , SquareItem *> rects;
	
	{
		SquareItem *currentSquareItem = d->squares[piece->currentSquare()];
		
		foreach(QGraphicsItem *it, items)
		{
			if(SquareItem*square =  qgraphicsitem_cast<SquareItem*>(it))
			{
				if( square == currentSquareItem ) continue;
				
				QRectF intersect = square->sceneBoundingRect().intersect(piece->sceneBoundingRect());
				
				rects.insert( intersect.height() * intersect.width(), square );
			}
		}
	}
	
	QList<double> heights = rects.keys();
	
	double max = -1;
	
	foreach(double p, heights)
	{
		if( p > max )
		{
			max = p;
		}
	}
	
	if( max > 0 )
	{
		Game::Board board = d->game->board();
		
		Game::Square from = piece->currentSquare();
		Game::Square to = rects[max]->logicalIndex();
		
		Game::Move move(board, from, to);
		
		if ((to < 8 || to > 55) && (board.at(from) == Game::WhitePawn || board.at(from) == Game::BlackPawn))
		{
			qWarning("PROMOTION!!!!!!");
		}
		
		if (board.isLegal(move))
		{
			movePiece(piece, from, to);
			
			if (d->game->atEnd())
			{
				d->game->addMove(move);
			}
			else
			{
				qFatal("No variants for now...");
			}
			
			d->game->forward();
			
			if( move.isCastling() || move.isEnPassant() || move.isSpecial() )
			{
				drawBoard(); // FIXME: fix this lazy way! =P
			}
			
			return true;
		}
		else
		{
			qWarning("Illegal move!");
		}
	}
	
	return false;
}

}
