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

#ifndef KLTABLE_H
#define KLTABLE_H

#include <qtable.h>

/**
 * Table generica de KLudoteca
 * @author CetiSoft
*/
class KLTable : public QTable
{
	Q_OBJECT
	public:
		/**
		 * Construye una tabla con el numero de filas y columnas indicados
		 * @param rows 
		 * @param cols 
		 * @param parent 
		 * @param name 
		 * @return 
		 */
		KLTable(int rows, int cols, QWidget *parent = 0, const char *name = 0);
		/**
		 * Destructor
		 * @return 
		 */
		~KLTable();

		/**
		 * Captura el evento cuando se redimensiona la tabla
		 * @param e 
		 */
		void resizeEvent ( QResizeEvent * e );
		
		/**
		 * Pone un titulo a la columna
		 * @param col 
		 * @param text 
		 */
		void setHeaderHText(int col, const QString &text);
		
		
		/**
		 * Dibuja los contenidos de la tabla
		 * @param p 
		 * @param cx 
		 * @param cy 
		 * @param cw 
		 * @param ch 
		 */
		void drawContents ( QPainter * p, int cx, int cy, int cw, int ch );
		
		
		/**
		 * Captura el evento de pintado de la tabla
		 * @param event 
		 */
		void paintEvent ( QPaintEvent * event );
		
		void paintRow(int row, const QColor &color);
		
		void paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg);
		
		QString getColumnTextHeader(int column);
				
	public slots:
		/**
		 * Arregla el tamño
		 */
		void fixSize();
		/**
		 * Distrubuye el ancho de las columnas con el ancho de toda la tabla
		 */
		void expandTableSize();
		/**
		 * Redimenciona las filas de una tabla
		 * @param rows 
		 */
		void redimensionTable(int rows);
		/**
		 * inserta datos a una fila
		 * @param data 
		 * @param row 
		 */
		void insertRowData(const QStringList &data, int row = -1);
		/**
		 * Pone un texto para mostrar cuando la tabla esta vacia
		 * @param text 
		 */
		void setViewportText(const QString &text);
		
	private:
		QString m_text;
		int m_specialRow;
		QColor m_specialRowColor;
};

#endif
