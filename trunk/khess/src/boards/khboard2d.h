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

#ifndef KHBOARD2D_H
#define KHBOARD2D_H


#include <qimage.h> 
#include "khboardbase.h" 
#include "khimagemanager.h"
#include "khgame.h"
#include <kpixmap.h>

#include "khficsmove.h"
#include "khmatch.h"

class KHBoard2D;

class KHBoard2D : public KHBoardBase
{
	Q_OBJECT
	public:
		KHBoard2D( QWidget * parent = 0, KHMatch *m = 0, const char * name = 0, WFlags f = 0);
		~KHBoard2D();
		
		virtual void loadBoardTheme(const QString &name = QString::null);
		virtual void loadFiguresTheme(const QString &name = QString::null);
		
		void loadThemeItem(KHImageManager::Images imageIndex, const QString &URL);
		void redrawAll();
		QPoint coords( const int &rank, const int &file );
		void myBlit( const QPoint &dp, const QPaintDevice *src, const QRect &sr );
		QPixmap getFigure(const int &pos);
		int position ( const QPoint &aPint );
		bool isSprite(const int &pos);
		KHGame *game();
		
	public slots:
		void drawPosition( int pos );
		void drawSprites();
		void drawMove( const KHChessMove &chessMove, const bool &reverse=FALSE );
		void setBoard(const QString &board);
		void doMove(KHChessMove *);
		void showPromotionDialog();
		
	signals:
		void moved(const KHChessMove &);
		
	private:
		class PrivateSprite
		{
			public: 
				PrivateSprite();
				~PrivateSprite();
				void setFade();
				
				bool m_restore;
				int m_steps;
				int m_boardPositionOrigin;
				int m_destination;
				int m_targetTaken;
				QPoint m_originPoint;
				QPoint m_currentPoint;
				QPoint m_lastPoint;
				QPoint m_destinationPoint;
				KPixmap m_sprite;
				KPixmap m_flipFrame;
		};
		
		QPtrList<KHBoard2D::PrivateSprite> m_spriteList;
		
		QPixmap m_drawer;
		QImage m_squareDark;
		int m_squareSize;
		int m_boardSize;
		int m_sideSquareSize;
		bool m_haveBorder;
		QRect m_rectBoard; // rectangle containing the Board
		
		KHImageManager m_imageManager;
		bool m_init;
		
		QPoint m_pressPoint;
		QPoint m_currentPoint;
		int m_boardPosition;
		QPixmap m_pieceTouched;
		
		PrivateSprite *m_dragSprite;
		
		int m_premoveFrom;
		int m_premoveTo;
		
	protected:
		virtual void paintEvent ( QPaintEvent * e);
		virtual void resizeEvent( QResizeEvent* e);
		virtual void mouseReleaseEvent( QMouseEvent *event );
		virtual void mousePressEvent( QMouseEvent *event );
		virtual void mouseMoveEvent( QMouseEvent *event );
		virtual void wheelEvent( QWheelEvent* );
};

#endif
