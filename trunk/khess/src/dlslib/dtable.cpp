/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                        *
 *   krawek@gmail.com                                       	   *
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

#include "dtable.h"
#include <qpainter.h>
#include <qsimplerichtext.h>
#include <qapplication.h>

DTable::DTable(int rows, int cols, QWidget *parent, const char *name)
	: QTable(rows, cols, parent, name), m_text(""), m_textSize(70)
{
	this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	expandTableSize();
	this->horizontalHeader()->setClickEnabled ( true);
// 	fixSize();
// 	setMinimumWidth(size().width());
}

DTable::~DTable()
{
}

void DTable::resizeEvent(QResizeEvent * e )
{
// 	qDebug(" DTable::resizeEvent");
	expandTableSize();
	QFrame::resizeEvent(e);
}

void DTable::expandTableSize()
{
// 	qDebug(" DTable::expandTableSize");
	for (int i = 0; i < numCols(); i++)
	{
		adjustColumn(i);
	}
	hint = size();

	int totalWidth = sizeHint().width();
	int colWidth = totalWidth;
// 	if((numCols() - 1) != 0)
	{
		colWidth = totalWidth / numCols();
	}
	for(int i = 0; i < numCols(); i++)
	{
		setColumnWidth(i, colWidth-5);
	}

// 	setMinimumSize(size());
}

void DTable::fixSize()
{
// 	qDebug(" DTable::fixSize");
	for (int i = 0; i < numCols(); i++)
	{
		adjustColumn(i);
	}
// 	vp->repaint();
// 	setMinimumSize(size());
// 	this->adjustSize();
// 	this->resize(this->sizeHint());

}

void DTable::setHeaderHText(int col, const QString &text)
{
// 	qDebug(" DTable::setHeaderHText");
	if ( col > numCols() )
		return;
	QHeader *header = this->horizontalHeader();
	header->setLabel(col, text);
	adjustColumn(col);
}

void DTable::insertRowData(const QStringList &data, int row)
{
// 	qDebug(" DTable::insertRowData");
	if( data.count() > numCols() || row < -1 )
		return;
	
	if(row == -1)
	{
		row = numRows();
		redimensionTable(numRows()+1);
	}
	for(uint i = 0; i < data.count(); i++)
	{
		setText(row, i, data[i]);
	}
}

void DTable::redimensionTable(int rows)
{
// 	qDebug(" DTable::redimensionTable");
	int cRows = numRows();
	if (rows > cRows)
	{
		insertRows(cRows, rows-cRows);
	}
	else if (rows < cRows)
	{
		int size = cRows-rows;
		if (size >= 0)
		{
			QMemArray<int> remove(size);
			int cont = 0;
			for(int i = rows; i < cRows; i++)
			{
				remove[cont] = i;
				cont++;
			}
			removeRows(remove);
		}
	}
	repaint();
	
}

void DTable::setViewportText(const QString &text, int size)
{
	m_textSize = size;
	m_text = text;
}

void DTable::drawContents( QPainter * p, int cx, int cy, int cw, int ch )
{
// 	qDebug("drawContents");
	if ( numRows() == 0 )
	{
		if ( !m_text.isEmpty() )
		{
			QSimpleRichText t( QString(
					"<div align=center>"
					"<h1>"+m_text+"</h1>"
					"</div>" ), QFont("Helvetica", m_textSize, QFont::Bold) );
			t.setWidth( clipper()->width() - 50 );
			const uint w = clipper()->width() - 20;
			const uint h = clipper()->height() - 20;
		
			p->setBrush( colorGroup().background() );
			p->drawRoundRect( 15, 15, w, h, (8*200)/w, (8*200)/h );
			t.draw( p, (w/2)-(t.width()/2), (h/2)-(t.height()/2), QRect(), colorGroup() );
		}
	}
	else
	{
		QTable::drawContents( p, cx, cy, cw, ch );
	}
	clipper()->repaint();
}

void DTable::paintEvent ( QPaintEvent * event )
{
// 	qDebug("paintEvent");
	QTable::paintEvent(event);
	viewport()->repaint();
}

// void DTable::viewportPaintEvent(QPaintEvent * pe )
// {
// 	qDebug("viewportPaintEvent");
// 	QTable::viewportPaintEvent(pe);
// }

QSize DTable::sizeHint () const
{
	return hint;
}

