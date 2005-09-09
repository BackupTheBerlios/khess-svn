/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   David Cuadrado  krawek@gmail.com                                      *
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

#ifndef KHSELECTFACEBUTTON_H
#define KHSELECTFACEBUTTON_H

#include <kpushbutton.h>

#define MAX_IMAGE_SIZE 100

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHSelectFaceButton : public KPushButton
{
	Q_OBJECT
	public:
		KHSelectFaceButton(QWidget *parent = 0, const char *name = 0);
		KHSelectFaceButton(const QPixmap &defaultPixmap, QWidget *parent = 0, const char *name = 0);
		~KHSelectFaceButton();
		virtual void setPixmap ( const QPixmap & pix);
		
	private slots:
		void selectAFace();
		
	private:
		void setup();

};

#endif
