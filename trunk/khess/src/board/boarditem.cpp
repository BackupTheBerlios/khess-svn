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

namespace Board {

class SquareItem : public QGraphicsItem
{
	public:
		
		enum { Type = UserType + 1 };
		SquareItem(QGraphicsItem *board, bool white);
		~SquareItem();
		
		
		void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
		
		QRectF boundingRect () const;
		
		int type () const
		{
			return Type;
		};
		
	private:
		bool m_white;
};

SquareItem::SquareItem(QGraphicsItem *board, bool white) : QGraphicsItem(board), m_white(white)
{
	
}

SquareItem::~SquareItem()
{
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
	QVector< QVector<SquareItem *> > squares;
	Game::Board gameBoard;
	
};

BoardItem::BoardItem( QGraphicsItem * parent , QGraphicsScene * scene ) : QGraphicsItem(), d( new Private)
{
	d->squares.resize(8);
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			bool white = ((j+i)%2 == 0);
			SquareItem *square = new SquareItem( this, white);
			square->setPos(i*square->boundingRect().width(), j *square->boundingRect().height());
			
			d->squares[i].append(square);
		}
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

void BoardItem::setGameBoard(const Game::Board &board)
{
	d->gameBoard = board;
	
	for(int i = 0; i < 64; i++)
	{
		int x = i % 8;
		int y = 7 - i / 8;
		
		Game::Piece pieceType = d->gameBoard.at(i);
		
		qDebug() << pieceType;
		
		if(pieceType != 0 && pieceType != 255)
		{
			PieceItem *piece = new PieceItem(this, pieceType);
			scene()->addItem(piece);
			
			setPiece(piece, y, x);
		}
	}
}

void BoardItem::setPiece(PieceItem * piece, int row, int col )
{
	piece->setParentItem(this);
	d->squares[col][row]->setZValue(1);
	piece->setPos(d->squares[col][row]->pos());
}

bool BoardItem::reposition(PieceItem* piece)
{
	QList<QGraphicsItem *> items = piece->collidingItems();
	
	QMap<double , SquareItem *> rects;
	
	foreach(QGraphicsItem *it, items)
	{
		if(SquareItem*square =  qgraphicsitem_cast<SquareItem*>(it))
		{
			QRectF intersect = square->sceneBoundingRect().intersect(piece->sceneBoundingRect());
			
			qDebug() << intersect;
			rects.insert( intersect.height() * intersect.width(), square );
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
		piece->setPos(rects[max]->pos());
		return true;
	}
	
	return false;
}

}
