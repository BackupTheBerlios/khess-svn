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

#include "pieceitem.h"
#include "boarditem.h"


#include <QSvgRenderer>
#include <QFile>
#include <QDebug>

namespace Board {

struct PieceItem::Private
{
	int z;
	BoardItem *board;
	QPointF currentPos;
	
	Game::Piece pieceType;
	Game::Square currentSquare;
};

PieceItem::PieceItem(BoardItem *board, const Game::Piece &type): QGraphicsSvgItem(), d(new Private)
{
	setFlag(QGraphicsItem::ItemIsMovable, true);
	
	d->pieceType = type;
	d->currentSquare = Game::InvalidSquare;
	
	chooseSvg();
	setZValue(10);
	d->board = board;
}


PieceItem::~PieceItem()
{
	delete d;
}

Game::Piece PieceItem::pieceType() const
{
	return d->pieceType;
}

Game::Square PieceItem::currentSquare() const
{
	return d->currentSquare;
}

void PieceItem::setCurrentSquare(const Game::Square &square)
{
	d->currentSquare = square;
}

void PieceItem::chooseSvg()
{
	QSvgRenderer *renderer = new QSvgRenderer;
	QString fileName;
	
	switch(d->pieceType)
	{
		case Game::BlackPawn:
		{
			fileName = "black_pawn.svg";
		}
		break;
		case Game::BlackRook:
		{
			fileName = "black_rook.svg";
		}
		break;
		case Game::BlackKnight:
		{
			fileName = "black_knight.svg";
		}
		break;
		case Game::BlackBishop:
		{
			fileName = "black_bishop.svg";
		}
		break;
		case Game::BlackQueen:
		{
			fileName = "black_queen.svg";
		}
		break;
		case Game::BlackKing:
		{
			fileName = "black_king.svg";
		}
		break;
		
		case Game::WhitePawn:
		{
			fileName = "white_pawn.svg";
		}
		break;
		case Game::WhiteRook:
		{
			fileName = "white_rook.svg";
		}
		break;
		case Game::WhiteKnight:
		{
			fileName = "white_knight.svg";
		}
		break;
		case Game::WhiteBishop:
		{
			fileName = "white_bishop.svg";
		}
		break;
		case Game::WhiteQueen:
		{
			fileName = "white_queen.svg";
		}
		break;
		case Game::WhiteKing:
		{
			fileName = "white_king.svg";
		}
		break;
	}
	
	if ( !fileName.isEmpty() )
	{
		QFile f(fileName);
		
		if( f.open(QIODevice::ReadOnly | QIODevice::Text ) )
		{
			renderer->load(f.readAll());
			setSharedRenderer(renderer);
		}
		else
		{
			qWarning("Cannot open file %s", fileName);
		}
	}
}


void PieceItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	d->z = zValue();
	setZValue(100);
	
	d->currentPos = pos();
}


void PieceItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	setZValue(d->z);
	
	if(!d->board->reposition(this))
		setPos(d->currentPos);
}

QVariant PieceItem::itemChange ( GraphicsItemChange change, const QVariant & value )
{
	return QGraphicsSvgItem::itemChange (  change,  value );
}

}


