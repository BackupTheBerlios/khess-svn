/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado - krawek@gmail.com               *
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

#include "khboardview.h" 

#include <qpainter.h>
#include <qlayout.h>

#include <kmessagebox.h>
#include <krun.h>
#include <klocale.h>
#include <qvbox.h>

#include "khclock.h"

KHBoardView::KHBoardView(const QString &caption, QWidget *parentWidget, KHMatch *match, const char *name, WFlags f) : KMdiChildView(caption, parentWidget, name, f), DCOPObject("KhessIface"), m_playerStatus(0)
{
	m_layout = new QHBoxLayout(this);
// 	m_layout->setAutoAdd(true);

	QVBox *m_boardContainer = new QVBox(this);
	m_boardContainer->setMargin(30);
	
	m_board = new KHBoard2D(m_boardContainer, match);
	connect(m_board->game(), SIGNAL(moved(const KHChessMove &)), this, SIGNAL(moved(const KHChessMove &)));
	
	m_layout->addWidget(m_boardContainer, 0,Qt::AlignAuto);
	
	m_board->resize( sizeHint() );
	
	setPaletteBackgroundColor( /*0x78631C*/ Qt::black );
}

KHBoardView::~KHBoardView()
{
}

void KHBoardView::print(QPainter *p, int height, int width)
{
}

void KHBoardView::debug(QString text )
{
	kdDebug() << text << endl;
	m_board->setBoard(text);
	m_board->redrawAll();
}

void KHBoardView::doMove(KHChessMove *move)
{
	kdDebug() << "KHBoardView::doMove" << endl;
	
	m_board->doMove(move);
}

void KHBoardView::slotSetTitle(const QString& title)
{
	emit signalChangeCaption(title);
}

void KHBoardView::resizeEvent(QResizeEvent *e)
{
}

void KHBoardView::setPlayerStatus(KHPlayersStatus *pstatus)
{
	m_playerStatus = pstatus;
}

KHPlayersStatus *KHBoardView::playerStatus()
{
	return m_playerStatus;
}

void KHBoardView::showEvent(QShowEvent *e)
{
	if(m_playerStatus)
	{
		m_playerStatus->show();
	}
}


#include "khboardview.moc"
