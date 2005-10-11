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

#ifndef KHGAME_H
#define KHGAME_H

#include <qobject.h>
#include <qvaluevector.h>

#include "figure.h"
#include "position.h"
#include "khchessmove.h"
#include "khficsmove.h"
#include "khmatch.h"

/**
 * En esta clase esta la logica del juego
 * @author David Cuadrado <krawek@gmail.com
*/
class KHGame : public QObject
{
	Q_OBJECT
	public:
		enum GameType
		{
			Standard = 0
		};
		
		KHGame(KHMatch *match = 0);
		~KHGame();

		int pointer( const int &file, const int &rank ) const;
		
		void setBoard( const QString &board=QString::null, const short ppf=-2 );
		
		Figure getFigureAt(int boardPos);
		Position getPosition(int boardPos);
		
		int getNoteAt(int boardPos);
		void hashMoves( const int &man, const bool &recursion = false );
		
		QPoint getFileAndRank(const KHChessMove &chessMove);
		void setNoteFor(int position, Position::NoteType note);
		
		int getKing( const bool band );
		void checkNormal();
		
		bool isPaused();
		
		void doMove(KHChessMove *move);
		
		void writeSAN();
		void writeCAN();
		bool parseCAN( bool band );
		bool parseSAN();
		
		bool isHumanTurn();
		bool currentBand();
		void setPromotion(const QChar &c);
		
	private:
		bool init(const int &type = Standard);
		void initStandard();
		bool move();
		bool moveIt();
		bool isChessman( const int figure );
		
	protected:
		int calcPointer( int file, int rank );
		void hashMoves( const bool &recursion=false );
		
	public slots:
		void slotSelect( int position );
		void clearSelections();
		void clearBoard();
		
	signals:
		void redrawPosition(int position);
		void drawMove(const KHChessMove &chessMove, const bool &reverse);
		void moved(const KHChessMove &);
		void promotion();
		
		
	protected:
		bool m_isPaused;
		
	private:
		QValueVector<Position> m_boardCore;
		QValueVector<Figure> m_figuresOnBoard;
		uint m_moveCounter;
		KHChessMove m_move;
		KHChessMove m_premove;
		bool m_isMoved, m_isPremoved;
		bool m_currentBand;
		int m_enPassant[2];
		int m_manPtr;
		
		QValueVector<Position> hash;
		QValueVector<Position> hashBackup;
		QValueVector<Figure> chessmanBackup;
		KHMatch *m_match;
		
		const int CF_King;
		const int CF_RookQ;
		const int CF_RookK;

};

#endif
