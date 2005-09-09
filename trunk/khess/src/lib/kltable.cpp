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

#include "kltable.h"
#include <klocale.h>
#include <qpainter.h>
#include <qsimplerichtext.h>
#include <qapplication.h>
#include <iostream>

KLTable::KLTable(int rows, int cols, QWidget *parent, const char *name)
	: QTable(rows, cols, parent, name), m_text(""), m_specialRow(-1)
{
	this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
 	expandTableSize();
}

KLTable::~KLTable()
{
}

void KLTable::resizeEvent(QResizeEvent * e )
{
	expandTableSize();
	QFrame::resizeEvent(e);
}

void KLTable::expandTableSize()
{
	int totalWidth = clipper()->width();
	
	int columns = numCols();

	if ( columns == 0 )
	{
		return;
	}

	int colWidth = totalWidth / columns;
	
	for(uint i = 0; i < numCols(); i++)
	{
		setColumnWidth(i, colWidth);
	}
}

void KLTable::fixSize()
{
	for (uint i = 0; i < numCols(); i++)
	{
		adjustColumn(i);
	}
	this->adjustSize();
	this->resize(this->sizeHint());
}

void KLTable::setHeaderHText(int col, const QString &text)
{
	if ( col > numCols() )
		return;
	QHeader *header = this->horizontalHeader();
	header->setLabel(col, text);
	adjustColumn(col);
}

void KLTable::insertRowData(const QStringList &data, int row)
{
	if( data.count() > numCols() || row < -1 )
		return;
	
	if(row == -1)
	{
		row = numRows();
		redimensionTable(numRows()+1);
	}
	
	for(uint i = 0; i < data.count(); i++)
	{
// 		std::cout << QString("Insertando a tabla %1 en fila %2 columna %3").arg(name()).arg(row).arg(i) << " dato " << data[i] << std::endl;
		setText(row, i, data[i]);
	}
}

void KLTable::redimensionTable(int rows)
{
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
}

void KLTable::setViewportText(const QString &text)
{
	m_text = text;
}

void KLTable::drawContents( QPainter * p, int cx, int cy, int cw, int ch )
{
// 	qDebug("drawContents");
	if ( numRows() == 0)
	{
		if ( !m_text.isEmpty() )
		{
			const uint w = clipper()->width() - 20;
			const uint h = clipper()->height() - 20;
			
			QSimpleRichText t( QString(
					"<div align=center>"
					"<h1>"+m_text+"</h1>"
					"</div>" ), QFont("Helvetica", h/7, QFont::Bold) );
		
			t.setWidth( clipper()->width() - 50 );
		
			p->setBrush( colorGroup().background() );
			p->drawRoundRect( 15, 15, w, h, (8*200)/w, (8*200)/h );
			t.draw( p, 60, h/2, QRect(), colorGroup() );
		}
	}
	else
	{
		QTable::drawContents( p, cx, cy, cw, ch );
	}
	clipper()->repaint();
}

void KLTable::paintEvent ( QPaintEvent * event )
{
// 	qDebug("paintEvent");
	QTable::paintEvent(event);
	expandTableSize();
	viewport()->repaint();
}

void KLTable::paintRow(int row, const QColor &color)
{
	m_specialRow = row;
	m_specialRowColor = color;
}

void KLTable::paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg)
{
	QColorGroup mycg = cg;
	if (row == m_specialRow)
	{
		mycg.setColor(QColorGroup::Base, m_specialRowColor);
		QTable::paintCell(p, row,col, cr, selected, mycg);
	}
	else
	{
		QTable::paintCell(p, row,col, cr, selected, mycg);
	}
}

QString KLTable::getColumnTextHeader(int column)
{
	return horizontalHeader()->label(column);
}

#include "kltable.moc"
