/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado - krawek@gmail.com               *
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


#ifndef _KHBOARDVIEW_
#define _KHBOARDVIEW_

#include <qwidget.h>
#include <khessiface.h>
#include <qhbox.h>
#include <kdebug.h>

#include "khplayerinfobox.h"
#include "khboard2d.h"
#include "khficsmove.h"

#include "khplayersstatus.h"

class QPainter;

/**
 * This is the board view class for Khess.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 *
 * @short Board view
 * @author David Cuadrado <krawek@gmail.com>
 * @version 0.1
 */

class KHBoardView : public QFrame, public KhessIface
{
	Q_OBJECT
	public:
		/**
	 	* Default constructor
	 	*/
		KHBoardView(QWidget *parentWidget=0L, KHMatch *match =0 , const char *name=0L, WFlags f=0);

		/**
		 * Destructor
	 	 */
		virtual ~KHBoardView();

    		 /**
		 * Print this view to any medium -- paper or not
       		 */
		void print(QPainter *, int height, int width);
		
		void debug(QString text);
		
		void setPlayerStatus(KHPlayersStatus *pstatus);
		KHPlayersStatus *playerStatus();
		
	public slots:
		void doMove(KHChessMove *);
		
	protected:
		void resizeEvent(QResizeEvent *e);

	signals:
		/**
		* Use this signal to change the content of the statusbar
		*/
		void signalChangeStatusbar(const QString& text);

		/**
		* Use this signal to change the content of the caption
		*/
		void signalChangeCaption(const QString& text);
		
		void moved(const KHChessMove &);
		

	private slots:
		void slotSetTitle(const QString& title);
		
	protected:
		void showEvent(QShowEvent *e);
		
	private:
		QHBoxLayout *m_layout;
		KHBoard2D *m_board;
		KHPlayersStatus *m_playerStatus;
};

#endif
