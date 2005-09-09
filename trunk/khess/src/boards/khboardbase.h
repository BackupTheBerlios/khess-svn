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

#ifndef KHBOARDBASE_H
#define KHBOARDBASE_H

// Qt
#include <qframe.h>
#include <qpixmapcache.h>

#include <cmath>
#include "khgame.h"

#define IMAGE_MAX 144
#define IMAGE_MIN 32

/**
	@author David Cuadrado <krawek@gmail.com>
*/

class KHBoardBase : public QWidget
{
	Q_OBJECT
	public:
		KHBoardBase(KHMatch *m = 0, QWidget *parent = 0, const char *name = 0);
		~KHBoardBase();
		
		bool orientation();
		void setLocalBand( bool );
		
		virtual void loadBoardTheme(const QString &name = QString::null) = 0;
		virtual void loadFiguresTheme(const QString &name = QString::null) = 0;
		virtual void redrawAll() = 0;
		
	public slots:
		virtual void showPromotionDialog();
		
	signals:
		void promoted(QChar c);

	private:
		bool m_boardOrientation;
		
	protected:
		KHGame *m_game;
};

#endif
