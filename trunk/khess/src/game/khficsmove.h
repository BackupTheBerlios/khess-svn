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
#ifndef KHFICSMOVE_H
#define KHFICSMOVE_H

#include <khchessmove.h>

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHFicsMove : public KHChessMove
{
	public:
    		KHFicsMove();
    		~KHFicsMove();
		
		void setBoard(const QString &board);
		QString board();
		
		int icsMode();
		int icsPawnPushFile();
		int icsMoveCounter();
		bool icsOnMove(); // true is white
		bool icsClockTicking();
		
		int blackTime();
		int whiteTime();
		
		void setIcsMode(int );
		void setIcsPawnPushFile(int );
		void setIcsMoveCounter(int );
		void setIcsOnMove(bool ); // true is white
		void setIcsClockTicking(bool );
		
		void reset();
		void setTimes(int whitetime, int blacktime);
		
	private:
		int m_blackTime, m_whiteTime;
		
		QString m_board;
		int m_icsMode;
		int m_icsPawnPushFile;
		int  m_icsMoveCounter;
		bool m_icsOnMove; // true is white
		bool m_icsClockTicking;

};

#endif
