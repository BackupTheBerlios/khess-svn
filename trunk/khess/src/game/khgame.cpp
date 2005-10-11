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

#include "khgame.h"
#include <kdebug.h>

#include "khwarrior.h"

KHGame::KHGame(KHMatch *match) : QObject(), m_isPaused(false), m_match(match), CF_King(0x01), CF_RookQ(0x02), CF_RookK(0x04)
{
	init();
}

KHGame::~KHGame()
{
}

bool KHGame::init(const int &type)
{
	m_boardCore.resize(74);
	m_figuresOnBoard.resize(74);
	m_moveCounter = 0;
	
	for( uint tmp = 0; tmp < 74; tmp++ )
	{
		m_boardCore[tmp].setFile( tmp % 8 );
		m_boardCore[tmp].setRank( tmp >> 3 );
	}

	switch( type )
	{
		case Standard:
		{
			initStandard();
		}
		break;
	}
		
	return true;
}

void KHGame::initStandard()
{
	for( int tmp = 0; tmp < 32; tmp++ )
	{
		if( tmp < 16 )
		{
			m_figuresOnBoard[tmp] = Figure('P');
			m_figuresOnBoard[tmp].setRank(0);
		}
		else
		{
			m_figuresOnBoard[tmp] = Figure('p');
			m_figuresOnBoard[tmp].setRank(7);
		}
		switch( tmp % 16 )
		{
			case 0:
			{
				m_figuresOnBoard[tmp].setType(Figure::King);
				m_figuresOnBoard[tmp].setFile (4);
				break;
			}
			case 1:
			{
				m_figuresOnBoard[tmp].setType(Figure::Queen);
				m_figuresOnBoard[tmp].setFile(3);
				break;
			}
			case 2:
			{
				m_figuresOnBoard[tmp].setType(Figure::Bishop);
				m_figuresOnBoard[tmp].setFile (2);
				break;
			}
			case 3:
			{
				m_figuresOnBoard[tmp].setType(Figure::Bishop);
				m_figuresOnBoard[tmp].setFile (5);
				break;
			}
			case 4:
			{
				m_figuresOnBoard[tmp].setType ( Figure::Knight);
				m_figuresOnBoard[tmp].setFile ( 1);
				break;
			}
			case 5:
			{
				m_figuresOnBoard[tmp].setType ( Figure::Knight);
				m_figuresOnBoard[tmp].setFile ( 6);
				break;
			}
			case 6:
			{
				m_figuresOnBoard[tmp].setType (Figure::Rook);
				m_figuresOnBoard[tmp].setFile (0);
				break;
			}
			case 7:
			{
				m_figuresOnBoard[tmp].setType ( Figure::Rook);
				m_figuresOnBoard[tmp].setFile ( 7);
				break;
			}
			default:
			{
				m_figuresOnBoard[tmp].setType (Figure::Pawn);
				m_figuresOnBoard[tmp].setFile( (tmp % 16 ) - 8 );
				if( m_figuresOnBoard[tmp].band() )
				{
					m_figuresOnBoard[tmp].setRank (1);
				}
				else
				{
					m_figuresOnBoard[tmp].setRank(6);
				}
				break;
			}
		}
		m_boardCore[ pointer( m_figuresOnBoard[tmp].file(), m_figuresOnBoard[tmp].rank() ) ].setManPtr(tmp);
	}
// 	CastleFlag[0] = CF_King + CF_RookQ + CF_RookK;
// 	CastleFlag[1] = CF_King + CF_RookQ + CF_RookK;
	m_currentBand = true;
}

bool KHGame::isHumanTurn()
{
	if (m_match->white()->type() == KHWarrior::Human && m_currentBand)
	{
		return true;
	}
	if ( m_match->black()->type() == KHWarrior::Human && !m_currentBand )
	{
		return true;
	}
	return false;
}

void KHGame::doMove(KHChessMove *move)
{	
	KHFicsMove *aMove = 0;
	aMove = dynamic_cast<KHFicsMove *>(move);
	
	if ( aMove ) // INTERNET
	{
		kdDebug() << "INTERNET MOVE" << endl;
		m_currentBand = !aMove->icsOnMove();
		
		kdDebug() << m_currentBand << endl;
		
		setBoard( aMove->board(), aMove->icsPawnPushFile() );
		m_moveCounter = aMove->icsMoveCounter();
		m_move = *aMove;
		
		int tmp = 0;
		while( tmp < 64 )
		{
			emit redrawPosition( tmp++ );
		}

		parseCAN( !m_currentBand );
		
		m_match->clock()->setNewTime( aMove->whiteTime(), aMove->blackTime(), !m_currentBand );
		
// 		if( aMove->icsClockTicking() )
// 		{
// 			m_match->clock()->play();
// 		}
// 		else
// 		{
// 			m_match->clock()->stop();
// 		}
		
	}
	else // ENGINE
	{
		kdDebug() << "ENGINE MOVE" << endl;
		m_move = *move;
		
		clearSelections();

		if( ! parseCAN( m_currentBand ) )
		{
			if( ! parseSAN() )
			{
				kdDebug() << "I can't move" << endl;
				return;
			}
		}

		m_isMoved = true;
		
		this->move();
	}
}

int KHGame::pointer( const int &file, const int &rank ) const
{
	if( ( file < 0 ) || ( file > 7 ) )
	{
		return -1;
	}
	if( ( rank < 0 ) || ( rank > 7 ) )
	{
		return -1;
	}
	return ( (  rank << 3 ) + file );
}

void KHGame::setBoard( const QString &board, const short ppf )
{
	clearBoard();
	
	kdDebug() << "setBoard: " << board << endl;
	char piece;
	int pos = 0, tmp2(0), cR(7), cF(0);

	if( board.length() < 64 )
	{
		kdWarning() << "logic::setBoard: Was passed a string that is less than 64 bytes long." << endl;
		return;
	}
	
	while( pos < 64 )
	{
		piece = board.at(pos++);
		
		kdDebug() << "*******PIECE: " << piece << endl;
		
		m_figuresOnBoard[tmp2] = Figure(piece);
		
		if( piece != '-' )
		{
			m_figuresOnBoard[tmp2].setRank(cR);
			m_figuresOnBoard[tmp2].setFile(cF);
			m_boardCore[ pointer( cF, cR ) ].setManPtr(tmp2);
			tmp2++;
		}
		
		cF++;
		if( cF == 8 )
		{
			cF = 0;
			cR--;
		}
		
// 		emit redrawPosition( pos );
		
	}
}

void KHGame::clearSelections()
{
	int tmp;

	for( tmp = 0; tmp < 74; tmp++ )
	{
		if( m_boardCore[tmp].note() != Position::NOTE_NONE )
		{
			m_boardCore[tmp].setNote(Position::NOTE_NONE);
			emit redrawPosition( tmp );
		}
	}
}

void KHGame::clearBoard()
{
	for( uint tmp = 0; tmp < m_figuresOnBoard.count(); tmp++ )
	{
		m_figuresOnBoard[tmp].setType(-1);
		m_boardCore[tmp].setManPtr(-1); // TODO: Reset function 
		m_boardCore[tmp].setNote(Position::NOTE_NONE);
	}
}

void KHGame::slotSelect( int position )
{
// 	kdDebug() << "SlotSelect: " << position << endl;
	bool preMoving(false);
	int tmp(0), selected(-1);
	bool army;

	if( m_isPaused )
	{
		return; // Duh
	}

	if( ( position < 0 ) || ( position > 74 ) )
	{
		return;
	}

	/* Clear all non-SELECT notes */
	for( tmp = 0; tmp < 74; tmp++ )
	{
		if( m_boardCore[tmp].note() != Position::NOTE_NONE )
		{
			if( m_boardCore[tmp].note() != Position::NOTE_SELECT )
			{
				m_boardCore[tmp].setNote(Position::NOTE_NONE);
				
				emit redrawPosition( tmp );
			}
			else
			{
				selected = tmp;
				break;
			}
		}
	}

	/* Check to make sure it's our turn to select. */
	if( !isHumanTurn() )
	{
		return;
	}

	/* If you left click on a selected square, it becomes unselected. */
	if( m_boardCore[position].note() == Position::NOTE_SELECT )
	{
		m_boardCore[position].setNote(Position::NOTE_NONE);
		emit redrawPosition( position );
		return;
	}

	/* Clicking on sideboards with no pieces get rejected. */
	if( ( position > 63 ) && ( m_figuresOnBoard[position].file() < 1 ) )
	{
		return;
	}

	/* If you premoved and then click somewhere else, the premove is cleared. */
	if( m_isPremoved )
	{
		m_isPremoved = false;
// 		playSound( Sound::Select );
// 		Board->setPremovePositions( -1, -1 );
		emit redrawPosition( pointer( m_premove.fromFile(), m_premove.fromRank() ) );
		emit redrawPosition( pointer( m_premove.toFile(), m_premove.toRank() ) );
	}

	/* Check to see if there is already a selected square. */
	if( selected != -1 )
	{
        /*
		If there is already a selected square, and you just clicked on another of your men,
		we'll change your selection.
	*/
		if( m_boardCore[position].manPtr() != -1 )
		{
			if( m_figuresOnBoard[ m_boardCore[selected].manPtr() ].band() == m_figuresOnBoard[ m_boardCore[position].manPtr() ].band() )
			{
				m_boardCore[selected].setNote(Position::NOTE_NONE);
				m_boardCore[position].setNote(Position::NOTE_SELECT);
				emit redrawPosition( selected );
				emit redrawPosition( position );
				return;
			}
		}

		if( preMoving )
		{
			kdDebug() << "PREMOVE" << endl;
			m_premove.reset();
			
			m_premove.copyData(m_boardCore[selected], m_boardCore[position]);
			
			m_isPremoved = true;

// 			Board->setPremovePositions( selected, position );
		}
		else
		{
			m_move.reset();
			
			
			kdDebug() << "SELECTED: " << selected << endl;
			kdDebug() << "POS: " << position << endl;
			
			m_move.copyData(m_boardCore[selected], m_boardCore[position]);
			
// 			for(uint i = 0; i < m_boardCore.count(); i++)
// 			{
// 				kdDebug() << m_boardCore[i].file() << endl;
// 			}
			m_isMoved = true;
			if( move() == false ) // AQUI SE HACE EL MOVIMIENTO!!
			{
				kdDebug() << "MOVE == FALSE" << endl;
				m_boardCore[selected].setNote(Position::NOTE_SELECT);
				emit redrawPosition( selected );
			}
		}
		return;
	}
	
	tmp = m_boardCore[position].manPtr();

	/* You can't select an empty square. */
	if( tmp == -1 )
	{
		return;
	}
	army = m_figuresOnBoard[ tmp ].band();

	/* You can't select your enemy */
	if( army != m_currentBand && ! preMoving )
	{
		return;
	}

	/* If your clicking on one of your chessmen, you can select it. */
	clearSelections();
	if( army /*&& ( WHITE_INPUT & PLAYERLOCAL )*/ )
	{
		m_boardCore[position].setNote(Position::NOTE_SELECT);
	} else
// 	if( ( army == BLACK ) && ( BLACK_INPUT & PLAYERLOCAL ) )
	{
		m_boardCore[position].setNote(Position::NOTE_SELECT);
	}
	
	emit redrawPosition( position );
	
// 	if( myResource->OPTION_Auto_Preview == true )
// 	{
// 		slot_Preview( position );
// 	}
// 	else
// 	{
// 	}
}

bool KHGame::move()
{
	int result(-1);
	int manPtr(-1);
	QString SAN;

	/* Clear selections from the board */
	clearSelections();

	/* Build Pointers */
	int mfr = m_move.fromRank();
	int fromPtr = ( mfr << 3 ) + m_move.fromFile();

	/* Skip all this if we're being called due to normal server moves */
	if( ( m_isMoved == true ) /*|| ( ICSGameMode == ICS_Movelist ) */)
	{
		manPtr = m_boardCore[fromPtr].manPtr();
		if( manPtr == -1 )
		{
			return false;
		}
		kdDebug() << "ANTES" << endl;
		if( moveIt() == false )
		{
			kdDebug() << "Can't move" << endl;
			return false;
		}
		else
		{
			if ( isHumanTurn() )
			{
				emit moved(m_move);
			}
		}
	}

	/* Check to see if the game is ended */
	/* 50 Move Rule? */
	if( m_moveCounter == 50 )
	{
// 		result = CMD_Result_Draw;
		kdDebug() << "DRAWWWW" << endl;
	}
    /*
	A Draw via 3 move repeat

	if( Record->isThreeMoveDraw() )
	{
	result = CMD_Result_Draw;
	Record->TAG_Result = "1/2-1/2";
	emit setStatusBar( GAME_DRAW );
}      */
    /*
	A Draw?

	Condition 1 = White Material Draw
	Condition 2 = Black Material Draw
	Condition 3 = Both sides agree to a draw
    */
	
// 	if( ( Logic->OnMove == BLACK && Logic->isDraw( WHITE ) ) ||
// 		     ( Logic->OnMove == WHITE && Logic->isDraw( BLACK ) ) ||
// 		     ( Draw_Offered[WHITE] && Draw_Offered[BLACK] ) )
// 	{
// 		result = CMD_Result_Draw;
// 	}
// 
// 	/* Is White under check? */
// 	if( Logic->isCheck( WHITE ) )
// 	{
// 		if( !Logic->isLegal( WHITE ) )
// 		{
// 			SAN += '#';
// 			result = CMD_Result_Black;
// 		}
// 		else
// 		{
// 			SAN += '+';
// 			soundType = Sound::Check;
// 		}
// 	}
// 
// 	/* Is Black under check? */
// 	if( Logic->isCheck( BLACK ) )
// 	{
// 		if( !Logic->isLegal( BLACK ) )
// 		{
// 			SAN += '#';
// 			result = CMD_Result_White;
// 		}
// 		else
// 		{
// 			SAN += '+';
// 			soundType = Sound::Check;
// 		}
// 	}

	emit drawMove( m_move, false );
	
	/* Check to make sure this isn't the starting position in an ICS match */
// 	if( QString( m_move.san() ) != "none" )
// 	{
// 		if( !SAN.isEmpty() )
// 		{
// 			m_move.setSan(SAN);
// 		}
// 		Record->history->add( m_move, Logic->board(), Clock->getCentiseconds( WHITE ), Clock->getCentiseconds( BLACK ) );
// 		Record->currentIndex = Record->history->count( MatchHistory::Move ) - 1;
// 		if( m_isMoved )
// 		{
			/* Send this move to engines and servers */
// 			emit sendCMD( Command( myID, CMD_Move, centiseconds( WHITE ), centiseconds( BLACK ),
// 				      m_move, Record->notation(false)->join(QString(" ")) ) );
// 		}
		/* Draw The Move */
// 		if( ICSGameMode != ICS_Movelist )
// 			emit drawMove( m_move, false );
// 	}

	/* Take care of changing turns, status messages, etc. */
	if( result == -1 )
	{
// 		Clock->Moved();
		if( m_isPaused )
		{
			m_match->clock()->stop();
		}
		else
		{
			m_match->clock()->play();
		}
		
		m_currentBand = !m_currentBand;

		/* Set Status Bar */
// 		if( Logic->OnMove == WHITE )
// 		{
// 			emit setStatusBar( WHITE_TURN );
// 		}
// 		if( Logic->OnMove == BLACK )
// 		{
// 			emit setStatusBar( BLACK_TURN );
// 		}

		/* Set Cursor */
// 		if( ( inputOnMove(true) == PLAYERLOCAL ) && ( inputOnMove() != PLAYERLOCAL ) )
// 		{
// 			Board->setCursor( myResource->CURSOR_Thinking );
// 		}
// 		else
// 		{
// 			Board->setCursor( myResource->CURSOR_Standard );
// 		}

		/* Deprecieate Draw Offers */
// 		if( Draw_Offered[WHITE] )
// 		{
// 			--Draw_Offered[WHITE];
// 		}
// 		if( Draw_Offered[BLACK] )
// 		{
// 			--Draw_Offered[BLACK];
// 		}

		/* Display NAG Anotation */
// 		if( m_move.NAG != 0 )
// 		{
// 			emit setStatusBar( COMMENT + m_move.NAG );
// 		}
	}
	else
	{
		/* End of Match */
// 		Board->setCursor( myResource->CURSOR_Standard );
// 		recvCMD( Command( myID, result ) );
// 		emit sendCMD( Command( myID, result ) );
// 		soundType = Sound::MatchOver;
	}
// 	Board->setNotation( Record->notation() );
// 
// 	/* Unless we're getting a batch of moves... */
// 	if( ICSGameMode != ICS_Movelist )
// 	{
// 		Board->redrawLights();
// 		playSound( soundType );
// 	}
// 
// 	/* Make the 'Retract' option available */
// 	if( ( inputOnMove() == PLAYERLOCAL ) && ( inputOnMove(true) == PLAYERPC ) && Record->history->count( MatchHistory::Move ) )
// 	{
// 		m_match->setItemEnabled( MENU_RETRACT, true );
// 	}
// 	else
// 	{
// 		m_match->setItemEnabled( MENU_RETRACT, false );
// 	}
// 
// 	/* Handle special cases */
// 	if( loading() )
// 	{
// 		loadTimer = 1; // Loading...
// 	}
// 	else
// 	{
// 		setModified( true );
// 		Record->refresh();
// 	}
// 	if( m_isPremoved )
// 	{
// 		m_isPremoved = false;
// 		Board->setPremovePositions( -1, -1 );
// 		m_move = preMove;
// 		JustMoved = true;
// 		move();
// 	}
// 
// 	/* Mail new move */
// 	if( ( Record->tags.find("Mode") == "EM" ) && ( !loading() ) )
// 	{
// 		Clock->Pause(); // Email
// 		int mailMove = KMessageBox::questionYesNo( 0, i18n("Would you like to send this move?"), i18n("Send Email?") );
// 
// 		if( mailMove == KMessageBox::Yes )
// 		{
// 			Record->sendEmail( Logic->OnMove );
// 			Logic->OnMove = -1;
// 		}
// 		else
// 		{
// 			/* Restore the GUI */
// 			retract();
// 			Logic->OnMove = !Logic->OnMove;
// 			if( Logic->OnMove == WHITE )
// 			{
// 				setStatusMessage( WHITE_TURN );
// 			}
// 			else
// 			{
// 				setStatusMessage( BLACK_TURN );
// 			}
// 			if( ( inputOnMove(true) == PLAYERLOCAL ) && ( inputOnMove() != PLAYERLOCAL ) )
// 			{
// 				Board->setCursor( myResource->CURSOR_Thinking );
// 			}
// 			else
// 			{
// 				Board->setCursor( myResource->CURSOR_Standard );
// 			}
// 			Board->setNotation( Record->notation() );
// 		}
// 	}

	return true;
}


bool KHGame::moveIt()
{
	kdDebug() << "MOVE IT" << endl;
// 	dlg_promote *ProDlg;
	int tmp;

	/* Don't use Pointer() to get these, as it's bounds checking kills *house moves */
	
	int mfr = m_move.fromRank();
	int mtf = m_move.toRank();
	
// 	kdDebug() << "MFR: " << mfr << " MTF: " << mtf << endl;
	
	int fromPtr = m_move.fromFile() + ( mfr << 3 );
	int toPtr = m_move.toFile() + ( mtf << 3 );
	
	kdDebug() << "fromPtr: " << fromPtr << " toPtr: " << toPtr << endl;
	m_move.debug();
	
	if( ( fromPtr < 0 ) || ( toPtr < 0 ) || ( fromPtr > 74 ) || ( toPtr > 74 ) )
	{
		kdDebug() << "ERROR" << endl;
		return false;
	}
	
	int manPtr = m_boardCore[fromPtr].manPtr();
	if( manPtr == -1 )
	{
		kdDebug() << "Invalid man" << endl;
		return false;
	}
	
	hashMoves( manPtr );
	
	/* Only proceed if this is a move */
	if( m_boardCore[toPtr].note() < Position::NOTE_MOVE )
	{
		kdDebug() << "NOT MOVE " << m_boardCore[toPtr].note() << endl;
		return false; // This depends on all moves being higher value than NOTE_MOVE,
    // while all non-moves are less.
	}

	/* Take care of moving the rook in a caste */
	if( m_boardCore[toPtr].note() == Position::NOTE_CASTLE )
	{
		if( m_move.toFile() == 6 )
		{
			for( tmp = 0; tmp < 64; tmp++ )
			{
				if( ( m_figuresOnBoard[tmp].band() == m_figuresOnBoard[manPtr].band() ) &&
				( m_figuresOnBoard[tmp].type() == Figure::Rook ) &&
				( m_figuresOnBoard[tmp].file() == 7 ) )
				{
					m_figuresOnBoard[tmp].setFile(5);
					m_boardCore[ pointer( 7, ( 7 * ( m_figuresOnBoard[tmp].band() == false ) ) ) ].setManPtr(-1);
					m_boardCore[ pointer( 5, ( 7 * ( m_figuresOnBoard[tmp].band() == false ) ) ) ].setManPtr(tmp);
					emit redrawPosition( pointer( 5, ( 7 * ( m_figuresOnBoard[tmp].band() == false ) ) ) );
					emit redrawPosition( pointer( 7, ( 7 * ( m_figuresOnBoard[tmp].band() == false ) ) ) );
					
					break;
				}
			}
		}
		if( m_move.toFile() == 2 )
		{
			for( tmp = 0; tmp < 64; tmp++ )
			{
				if( ( m_figuresOnBoard[tmp].band() == m_figuresOnBoard[manPtr].band() ) &&
				 ( m_figuresOnBoard[tmp].type() == Figure::Rook ) &&
				 ( m_figuresOnBoard[tmp].file() == 0 ) )
				{
					m_figuresOnBoard[tmp].setFile(3);
					m_boardCore[ pointer( 0, ( 7 * ( m_figuresOnBoard[tmp].band() == false ) ) ) ].setManPtr(-1);
					m_boardCore[ pointer( 3, ( 7 * ( m_figuresOnBoard[tmp].band() == false ) ) ) ].setManPtr(tmp);
					
					emit redrawPosition( pointer( 0, ( 7 * ( m_figuresOnBoard[tmp].band() == false ) ) ) );
					emit redrawPosition( pointer( 3, ( 7 * ( m_figuresOnBoard[tmp].band() == false ) ) ) );
					break;
				}
			}
		}
	}

	/* Handle the 50 Move Rule */
	m_moveCounter++;
	if( m_figuresOnBoard[manPtr].type() == Figure::Pawn )
	{
		m_moveCounter = 0;
	}
	
	if( m_boardCore[ toPtr ].manPtr() != -1 )
	{
		m_moveCounter = 0;
		m_move.setManTaken ( m_boardCore[ toPtr ].manPtr() );
	}

	/* Check for Pawn Promotion */
	if( ( m_move.toRank() == ( 7 * m_figuresOnBoard[manPtr].band() ) ) &&
		     ( m_figuresOnBoard[manPtr].type() == Figure::Pawn ) )
	{
		if( isHumanTurn() )
		{
			emit promotion();
		}
	}

	/* Write CAN & SAN Notation for this move */
	writeCAN();
	writeSAN();

	if( manPtr < 64 )
	{
		/* Make the move */
		m_figuresOnBoard[manPtr].setPosition(m_move);
		
		m_boardCore[fromPtr].setManPtr(-1);
		m_boardCore[toPtr].setManPtr(manPtr);
	}
	else
	{
		/* Drop a New Piece */
		for( tmp = 0; tmp < 64; tmp++ )
		{
			if( ! isChessman( tmp ) )
			{
				m_figuresOnBoard[manPtr].setFile( m_figuresOnBoard[manPtr].file()-1); // Drop Man count in Sideboard
				m_figuresOnBoard[tmp] = m_figuresOnBoard[manPtr]; // Create new man
				m_figuresOnBoard[tmp].setFile(m_move.toFile());
				m_figuresOnBoard[tmp].setRank(m_move.toRank());
				m_boardCore[toPtr].setManPtr(tmp);
				manPtr = tmp;
				break;
			}
		}
		if( tmp == 64 )
		{
			kdError() << "Logic::Move: Board full. Can not drop m_figuresOnBoard." << endl;
		}
	}

	/* Remove pawns taken en passant */
	if( m_boardCore[toPtr].note() == Position::NOTE_ENPASSANT )
	{
		m_moveCounter = 0;
		m_move.setManTaken( m_boardCore[ m_enPassant[ 1 - m_figuresOnBoard[manPtr].band() ] ].manPtr());
		
		m_boardCore[ m_enPassant[ 1 - m_figuresOnBoard[manPtr].band() ] ].setManPtr(-1);
	}

	/* Take care of en passant data */
	if( m_boardCore[toPtr].note() == Position::NOTE_PAWN_DOUBLE )
	{
		m_enPassant[ m_figuresOnBoard[manPtr].band() ] = toPtr;
	}
	m_enPassant[ 1 - m_figuresOnBoard[manPtr].band() ] = -1;

	/* Handle castle flags */
// 	if( m_figuresOnBoard[manPtr].Type == King )
// 		CastleFlag[ m_figuresOnBoard[manPtr].band() ] = 0;
// 	if( ( m_figuresOnBoard[manPtr].Type == Rook ) && ( m_move.fromFile == 0 ) )
// 		CastleFlag[ m_figuresOnBoard[manPtr].band() ] -= CF_RookQ;
// 	if( ( m_figuresOnBoard[manPtr].Type == Rook ) && ( m_move.fromFile == 7 ) )
// 		CastleFlag[ m_figuresOnBoard[manPtr].band() ] -= CF_RookK;

	return true;
}

void KHGame::writeSAN()
{
	int tmp, manPtr, toPtr, fromPtr;
	QString SAN;

	fromPtr = ( m_move.fromRank() << 3 ) + m_move.fromFile();
	toPtr = ( m_move.toRank() << 3 ) + m_move.toFile();
	manPtr = m_boardCore[fromPtr].manPtr();

	if( fromPtr > 63 )
	{
		/* Dropped piece = Special rules ( As Always ) */
#warning "Error?"
		SAN += QString("%1%2%3%4%5").arg(m_figuresOnBoard[manPtr].sanChar()).arg(char('@')).arg(char( m_move.toFile() + 97 )).arg( char(m_move.toRank() + 49 ));
		m_move.setSan(SAN);
		return;
	}

	if( manPtr == -1 )
	{
		return;
	}

	/* Check ambiguity for SAN notation */
    /*
	This calls hashMoves(), which writes on m_boardCore[],
	which we need intact for move()... so we need a backup
	copy of m_boardCore[]. Removing this breaks en passant moves!
    */
	QValueVector<Position> backup = QValueVector<Position>(m_boardCore);
	bool SANambig = false;
	bool SANambig2 = false;

	for( tmp = 0; tmp < 64; tmp++ )
	{
		if( tmp == manPtr )
			continue;
		if( !isChessman( tmp ) )
			continue;
		if( m_figuresOnBoard[tmp].band() == m_figuresOnBoard[manPtr].band() )
		{
			if( m_figuresOnBoard[tmp].type() == m_figuresOnBoard[manPtr].type() )
			{
				hashMoves( tmp );
				if( m_boardCore[toPtr].note() >= Position::NOTE_MOVE )
				{
					SANambig = true;
					if( m_figuresOnBoard[tmp].file() == m_figuresOnBoard[manPtr].file() )
					{
						SANambig2 = true;
					}
				}
			}
            		/* This IF was added to fix an ambiguity that occurs when a pawn on B file and a Bishop can attack the same spot */
			else
			{
				if( ( ( m_figuresOnBoard[manPtr].type() == Figure::Bishop ) && ( m_figuresOnBoard[tmp].type() == Figure::Pawn ) ) || ( ( m_figuresOnBoard[manPtr].type() == Figure::Pawn ) && ( m_figuresOnBoard[tmp].type() == Figure::Bishop ) ) )
				{
					if( ( m_figuresOnBoard[manPtr].file() == 1 ) || ( m_figuresOnBoard[tmp].file() == 1 ) )
					{
						hashMoves( tmp );
						if( m_boardCore[toPtr].note() >= Position::NOTE_MOVE )
						{
							SANambig = true;
							SANambig2 = true;
						}
					}
				}
			}
		}
	}
	/* Restore the backup. */
// 	copyPositions( backup, m_boardCore );

	m_boardCore = backup;
	
	if( ( ( m_boardCore[toPtr].note() == Position::NOTE_ATTACK ) ||
		       ( m_boardCore[toPtr].note() == Position::NOTE_ENPASSANT ) ) &&
		       ( m_figuresOnBoard[manPtr].type() == Figure::Pawn ) )
	{
		SANambig = true;
	}
	/* Write SAN Notation */
	if( m_boardCore[toPtr].note() == Position::NOTE_CASTLE )
	{
		if( m_move.toFile() == 6 )
			SAN = "O-O";
		if( m_move.toFile() == 2 )
			SAN = "O-O-O";
	}
	else
	{
		QChar ctmp = m_figuresOnBoard[manPtr].sanChar();
		
		if ( ctmp != 'P' )
		{
			SAN = ctmp;
		}
		
		if( SANambig == true )
		{
			SAN += char( m_move.fromFile() + 97 );
			if( SANambig2 )
				SAN += char( m_move.fromRank() + 49 );
		}
		
		if( ( m_boardCore[toPtr].note() == Position::NOTE_ATTACK ) || ( m_boardCore[toPtr].note() == Position::NOTE_ENPASSANT ) )
			SAN += 'x';
		SAN += char( m_move.toFile() + 97 );
		SAN += char( m_move.toRank() + 49 );
		
		switch( m_move.promote() )
		{
			case 'q':
				m_figuresOnBoard[manPtr].setType(Figure::Queen);
				SAN += "=Q";
				break;
			case 'b':
				m_figuresOnBoard[manPtr].setType(Figure::Bishop);
				SAN += "=B";
				break;
			case 'n':
				m_figuresOnBoard[manPtr].setType(Figure::Knight);
				SAN += "=N";
				break;
			case 'r':
				m_figuresOnBoard[manPtr].setType(Figure::Rook);
				SAN += "=R";
				break;
			default:
				break;
		}
	}
	m_move.setSan(SAN);
    
	kdDebug() << "SAN: " << SAN << endl;
}

void KHGame::writeCAN()
{
	QString can = QString("%1%2%3%4").arg(char(m_move.fromFile()+ 97)).arg(char(m_move.fromRank() + 49)).arg(char(m_move.toFile()+ 97)).arg(char(m_move.toRank() + 49));
	
	if( m_move.promote() != ' ' )
	{
		can += QString("%1").arg(m_move.promote());
	}
	m_move.setCan(can);
	
	kdDebug() << "CAN: " << can << endl;
}

bool KHGame::parseCAN( bool band )
{
	if( m_move.can() == "" )
	{
		return false;
	}
	
	/*
	For some reason some engines w/ CAN output
	express castling using SAN, not to name names... GNUChess v4!
	*/
	if( ( m_move.can()[0] == 'o' ) && ( m_move.can()[0] == 'O' ) )
	{
		m_move.setFromFile(4);
		if( QString( m_move.can() ).lower() == "o-o" )
			m_move.setToFile(6);
		else
			m_move.setToFile(2);
		if( band )
		{
			m_move.setFromRank(0);
			m_move.setToRank(0);
		}
		else
		{
			m_move.setFromRank(7);
			m_move.setToRank(7);
		}
		return true;
	}

	/* Standard notation parsing */
	if( ( m_move.can()[0] < 'a' ) || ( m_move.can()[0] > 'h' ) )
	{
		return false;
	}
	m_move.setFromFile(m_move.can().at(0).latin1() - 97);
	if( ( m_move.can()[1] < '1' ) || ( m_move.can()[1] > '8' ) )
	{
		return false;
	}
	m_move.setFromRank(m_move.can()[1].latin1() - 49);
	if( ( m_move.can()[2] < 'a' ) || ( m_move.can()[2] > 'h' ) )
	{
		return false;
	}
	m_move.setToFile( m_move.can()[2].latin1() - 97);
	if( ( m_move.can()[3] < '1' ) || ( m_move.can()[3] > '8' ) )
	{
		return false;
	}
	m_move.setToRank(m_move.can()[3].latin1() - 49);
	if( strlen( m_move.can() ) == 5 )
	{
		m_move.setPromote(m_move.can()[4]);
	}
	else
	{
		m_move.setPromote(' ');
	}
	return true;
}

bool KHGame::parseSAN( void )
{
	bool cband = m_currentBand ;

	signed char SANPtr(0), tmp(0);
	
	int len = m_move.san().length();
	
	m_move.setFromFile(-1);
	m_move.setFromRank(-1);
	m_move.setToFile(-1);
	m_move.setToRank(-1);
	m_move.setPromote(-1);
	m_move.setManTaken(-1);
	m_move.setNAG(0);
	
	kdDebug() << "Parsing: " << m_move.san() << endl;
	
	Figure::Type type = Figure::Pawn;

	while( SANPtr < len )
	{
		/* Parse a character */
		switch( m_move.san()[SANPtr].latin1() )
		{
			case 'K':
				if( SANPtr == 0 )
					type = Figure::King;
				else
					m_move.setPromote( 'k');
				break;
			case 'Q':
				if( SANPtr == 0 )
					type = Figure::Queen;
				else
					m_move.setPromote( 'q');
				break;
			case 'B':
				if( SANPtr == 0 )
					type = Figure::Bishop;
				else
					m_move.setPromote( 'b');
				break;
			case 'N':
				if( SANPtr == 0 )
					type = Figure::Knight;
				else
					m_move.setPromote( 'n');
				break;
			case 'R':
				if( SANPtr == 0 )
					type = Figure::Rook;
				else
					m_move.setPromote( 'r');
				break;
			case 'P':
				if( SANPtr == 0 )
					type = Figure::Pawn;
				break;
				/* Parse castle */
			case 'o':
			case 'O':
				if( SANPtr != 0 )
					break;
				type = Figure::King;
				if( cband )
					m_move.setToRank( 0);
				else
					m_move.setToRank( 7);
				if( QString( m_move.san() ).lower() == "o-o-o" )
					m_move.setToFile(2);
				if( QString( m_move.san() ).lower() == "o-o" )
					m_move.setToFile(6);
				break;
				/* Ignore some symbols... these fall through */
			case 'x':
			case '=':
			case '#':
			case '+':
			case '-':
				break;
				/* Handle annotations */
			case '!':
				m_move.setNAG(1);
				if( m_move.san()[SANPtr - 1] == '!' )
					m_move.setNAG(3);
				if( m_move.san()[SANPtr - 1] == '?' )
					m_move.setNAG(6);
				break;
			case '?':
				m_move.setNAG( 2);
				if( m_move.san()[SANPtr - 1] == '!' )
					m_move.setNAG(5);
				if( m_move.san()[SANPtr - 1] == '?' )
					m_move.setNAG(4);
				break;
			default:
				if( ( m_move.san()[SANPtr] >= '1' ) && ( m_move.san()[SANPtr] <= '8' ) )
				{
					if( m_move.toRank() != -1 )
					{
						m_move.setFromRank(m_move.toRank());
					}
					m_move.setToRank(m_move.san()[SANPtr].latin1() - 49);
					break;
				}
				if( ( m_move.san()[SANPtr] >= 'a' ) && ( m_move.san()[SANPtr] <= 'h' ) )
				{
					if( m_move.toFile() != -1 )
					{
						m_move.setFromFile(m_move.toFile());
					}
					m_move.setToFile(m_move.san()[SANPtr].latin1() - 97);
					break;
				}
// 				/* Unknown symbol... Can not process this chessMove */
				kdDebug() << "logic::ParseSAN: Unknown Symbol: " << m_move.san()[SANPtr] << endl;
				return false;
				break;
		}
		SANPtr++;
	}
	
	for( tmp = 0; tmp < 64; tmp++ )
	{
		if( m_figuresOnBoard[tmp].type() != type )
		{
			continue;
		}
		if( m_figuresOnBoard[tmp].band() != cband )
		{
			continue;
		}
		
		if( ( m_move.fromFile() != -1 ) && ( m_figuresOnBoard[tmp].file() != m_move.fromFile() ) )
		{
			continue;
		}
		if( ( m_move.fromRank() != -1 ) && ( m_figuresOnBoard[tmp].rank() != m_move.fromRank() ) )
		{
			continue;
		}
		if( !isChessman( tmp ) )
		{
			continue;
		}
		hashMoves( tmp );
		
		if( m_boardCore[ ( m_move.toRank() << 3 ) + m_move.toFile() ].note()  < Position::NOTE_MOVE )
		{
			continue;
		}
		m_move.setFromFile (m_figuresOnBoard[tmp].file());
		m_move.setFromRank ( m_figuresOnBoard[tmp].rank());
		break;
	}
	if( tmp == 64 )
	{
	// No valid move
		kdWarning() << "logic::ParseSAN: Invalid move: " << m_move.san() << endl;
		return false;
	}
	
	return true;
}


void KHGame::hashMoves( const int &man, const bool &recursion )
{
// 	kdDebug() << "HashMoves 1 " << m_manPtr << endl;
	int tmp = m_manPtr;
	m_manPtr = man;
	hashMoves( recursion );
	m_manPtr = tmp;
}

void KHGame::hashMoves( const bool &recursion )
{
// 	kdDebug() << "*****************HASH MOVES" << endl;
	/* Used for loops and positions */
	int ptr(0), tmp(0);

	/* Used to calculate a position relative to a given position */
	int dirF(0), dirR(0);
	int _castleFlag(0);

	/* Check for ghost chessmen */
	if( !isChessman(m_manPtr) )
	{
		kdDebug() << "NOT CHESSMEN" << endl;
		return;
	}

	/* Copy data into hash so we don't corrupt anything */	
 	hash = QValueVector<Position>(m_boardCore);

	/* Clear all NOTES */
	while( tmp < 74 )
	{
		hash[tmp++].setNote(Position::NOTE_NONE);
	}

	/* Men on the Sideboard get special attention */
	if( m_manPtr > 63 )
	{
		kdDebug() << "ERROR; SIDEBOARD" << endl;
		for( tmp = 0; tmp < 64; tmp++ )
		{
			if( ( m_figuresOnBoard[m_manPtr].type() == Figure::Pawn ) &&
						   ( ( hash[tmp].rank() == 0 ) || ( hash[tmp].rank() == 7 ) ) )
			{
				continue;
			}
			if( hash[tmp].manPtr() == -1 )
			{
				hash[tmp].setNote( Position::NOTE_MOVE);
			}
		}
	}
	else
	{
        /*
		This switch determines the piece type and creates a basic
		set of 'theoretical' moves for the piece. These moves then
		get a closer look in "The Monster" and are pruned if needed.
	*/
// 		kdDebug() << "FIGURE: " << m_figuresOnBoard[m_manPtr].type() << endl;
		switch( m_figuresOnBoard[m_manPtr].type() )
		{
			/* ROOK & QUEEN */
			case Figure::Rook:
			case Figure::Queen:
				/* Positive Rank Movement */
				for( tmp = 1; tmp < 8; tmp++ )
				{
					ptr = calcPointer( 0, tmp );
					if( ptr == -1 )
						break;
					if( hash[ptr].manPtr() == -1 )
					{
						hash[ptr].setNote(Position::NOTE_MOVE);
						continue;
					}
					if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
					{
						hash[ptr].setNote(Position::NOTE_ATTACK);
						break;
					}
					if( recursion == true )
					{
						hash[ptr].setNote(Position::NOTE_ATTACK);
					}
					break;
				}
				/* Negitive Rank Movement */
				for( tmp = -1; tmp > -8; tmp-- )
				{
					ptr = calcPointer( 0, tmp );
					if( ptr == -1 )
					{
						break;
					}
					if( hash[ptr].manPtr() == -1 )
					{
						hash[ptr].setNote(Position::NOTE_MOVE);
						continue;
					}
					if( m_figuresOnBoard[hash[ptr].manPtr() ].band() != m_figuresOnBoard[m_manPtr].band() )
					{
						hash[ptr].setNote(Position::NOTE_ATTACK);
						break;
					}
					if( recursion == true )
					{
						hash[ptr].setNote(Position::NOTE_ATTACK);
					}
					break;
				}
				/* Positive File Movement */
				for( tmp = 1; tmp < 8; tmp++ )
				{
					ptr = calcPointer( tmp, 0 );
					if( ptr == -1 )
					{
						break;
					}
					if( hash[ptr].manPtr() == -1 )
					{
						hash[ptr].setNote(Position::NOTE_MOVE);
						continue;
					}
					if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
					{
						hash[ptr].setNote( Position::NOTE_ATTACK );
						break;
					}
					if( recursion == true )
						hash[ptr].setNote( Position::NOTE_ATTACK );
					break;
				}
				/* Negative File Movement */
				for( tmp = -1; tmp > -8; tmp-- )
				{
					ptr = calcPointer( tmp, 0 );
					if( ptr == -1 )
						break;
					if( hash[ptr].manPtr() == -1 )
					{
						hash[ptr].setNote( Position::NOTE_MOVE);
						continue;
					}
					if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
					{
						hash[ptr].setNote( Position::NOTE_ATTACK);
						break;
					}
					if( recursion == true )
						hash[ptr].setNote( Position::NOTE_ATTACK);
					break;
				}
				if( m_figuresOnBoard[m_manPtr].type() == Figure::Rook )
					break;
				/* Bishop & Queen */
			case Figure::Bishop:
				/* NE Movement */
				for( tmp = 1; tmp < 8; tmp++ )
				{
					ptr = calcPointer( tmp, tmp );
					if( ptr == -1 )
						break;
					if( hash[ptr].manPtr() == -1 )
					{
						hash[ptr].setNote( Position::NOTE_MOVE);
						continue;
					}
					if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
					{
						hash[ptr].setNote( Position::NOTE_ATTACK);
						break;
					}
					if( recursion == true )
						hash[ptr].setNote( Position::NOTE_ATTACK);
					break;
				}
				/* NW Movement */
				for( tmp = -1; tmp > -8; tmp-- )
				{
					ptr = calcPointer( tmp, abs(tmp) );
					if( ptr == -1 )
						break;
					if( hash[ptr].manPtr() == -1 )
					{
						hash[ptr].setNote( Position::NOTE_MOVE);
						continue;
					}
					if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
					{
						hash[ptr].setNote( Position::NOTE_ATTACK);
						break;
					}
					if( recursion == true )
						hash[ptr].setNote( Position::NOTE_ATTACK);
					break;
				}
				/* SW Movement */
				for( tmp = -1; tmp > -8; tmp-- )
				{
					ptr = calcPointer( tmp, tmp );
					if( ptr == -1 )
						break;
					if( hash[ptr].manPtr() == -1 )
					{
						hash[ptr].setNote( Position::NOTE_MOVE);
						continue;
					}
					if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
					{
						hash[ptr].setNote( Position::NOTE_ATTACK);
						break;
					}
					if( recursion == true )
						hash[ptr].setNote( Position::NOTE_ATTACK);
					break;
				}
				/* SE Movement */
				for( tmp = -1; tmp > -8; tmp-- )
				{
					ptr = calcPointer( abs(tmp), tmp );
					if( ptr == -1 )
						break;
					if( hash[ptr].manPtr() == -1 )
					{
						hash[ptr].setNote( Position::NOTE_MOVE);
						continue;
					}
					if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
					{
						hash[ptr].setNote( Position::NOTE_ATTACK);
						break;
					}
					if( recursion == true )
						hash[ptr].setNote( Position::NOTE_ATTACK);
					break;
				}
				break;
				/* Knight */
			case Figure::Knight:
				for( tmp = 0; tmp < 8; tmp++ )
				{
					switch( tmp )
					{
						case 0:
							ptr = calcPointer( -1, 2 );
							break;
						case 1:
							ptr = calcPointer( 1, 2 );
							break;
						case 2:
							ptr = calcPointer( 2, 1 );
							break;
						case 3:
							ptr = calcPointer( 2, -1 );
							break;
						case 4:
							ptr = calcPointer( 1, -2 );
							break;
						case 5:
							ptr = calcPointer( -1, -2 );
							break;
						case 6:
							ptr = calcPointer( -2, -1 );
							break;
						case 7:
							ptr = calcPointer( -2, 1 );
							break;
						default:
							break;
					}
					if( ptr != -1 )
					{
						if( hash[ptr].manPtr() == -1 )
							hash[ptr].setNote( Position::NOTE_MOVE);
						else
						{
							if( recursion == true )
								hash[ptr].setNote( Position::NOTE_ATTACK);
							if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
								hash[ptr].setNote( Position::NOTE_ATTACK);
						}
					}
				}
				break;
				/* King */
			case Figure::King:
				dirF = -1;
				dirR = 1;
				while(1)
				{
					ptr = calcPointer( dirF, dirR );
					if( ptr != -1 )
					{
						if( hash[ptr].manPtr() == -1 )
							hash[ptr].setNote( Position::NOTE_MOVE);
						else
						{
							if( recursion == true )
								hash[ptr].setNote( Position::NOTE_ATTACK);
							if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
								hash[ptr].setNote( Position::NOTE_ATTACK);
						}
					}
					dirF++;
					if( dirF == 2 )
					{
						dirF = -1;
						dirR--;
					}
					if( dirR == -2 )
						break;
					if( ( dirR == 0 ) && ( dirF == 0 ) )
						dirF++;
				}
				/* Check for castles */
				if( recursion == false )
				{
					/* Can the King castle at all? */
// 					if( CastleFlag[ m_figuresOnBoard[ m_manPtr ].band() ] & CF_King )
					{
						dirR = 0;
						/* How about with the Queen's Rook? */
// 						if( CastleFlag[ m_figuresOnBoard[ ManPtr ].band() ] & CF_RookQ )
						{
							if( hash[ calcPointer( -1, dirR ) ].manPtr() == -1 )
							{
								if( hash[ calcPointer( -2, dirR ) ].manPtr() == -1 )
								{
									if( hash[ calcPointer( -3, dirR ) ].manPtr() == -1 )
									{
										hash[ calcPointer( -2, dirR ) ].setNote( Position::NOTE_CASTLE );
										_castleFlag |= CF_RookQ;
									}
								}
							}
						}
						/* King's Rook? */
// 						if( CastleFlag[ m_figuresOnBoard[ ManPtr ].band() ] & CF_RookK )
						{
							if( hash[ calcPointer( 1, dirR ) ].manPtr() == -1 )
							{
								if( hash[ calcPointer( 2, dirR ) ].manPtr() == -1 )
								{
									hash[ calcPointer( 2, dirR ) ].setNote( Position::NOTE_CASTLE) ;
									_castleFlag |= CF_RookK;
								}
							}
						}
					}
				}
				break;
				/* PAWN */
			default:
				/* Get direction of movement */
				if( m_figuresOnBoard[m_manPtr].band() )
				{
					dirR = 1;
				}
				else
				{
					dirR = -1;
				}
				if( recursion == false )
				{
					/* Forward 1 square */
					ptr = calcPointer( 0, dirR );
					if( ( ptr != -1 ) && ( hash[ptr].manPtr() == -1 ) )
					{
						hash[ptr].setNote( Position::NOTE_MOVE);
						tmp = 1 + ( 5 * ( m_figuresOnBoard[m_manPtr].band() == false ) );
						
// 						kdDebug() << "TMP: " << tmp << endl;
						
						if( m_figuresOnBoard[m_manPtr].rank() == tmp )
						{
							/* Forward 2 squares */
							dirR = dirR << 1;
							ptr = calcPointer( 0, dirR );
							if( ( ptr != -1 ) && ( hash[ptr].manPtr() == -1 ) )
								hash[ptr].setNote( Position::NOTE_PAWN_DOUBLE);
							dirR = dirR >> 1;
						}
					}
				}
				if( recursion == true )
				{
					/* Attack Left */
					ptr = calcPointer( -1, dirR );
					if( ptr != -1 )
						hash[ptr].setNote( Position::NOTE_ATTACK);
					/* Attack Right */
					ptr = calcPointer( 1, dirR );
					if( ptr != -1 )
						hash[ptr].setNote( Position::NOTE_ATTACK);
				}
				else
				{
					/* Attack Left */
					ptr = calcPointer( -1, dirR );
					if( ( ptr != -1 ) && ( hash[ptr].manPtr() != -1 ) )
					{
						if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
							hash[ptr].setNote( Position::NOTE_ATTACK);
					}
					/* Attack Right */
					ptr = calcPointer( 1, dirR );
					if( ( ptr != -1 ) && ( hash[ptr].manPtr() != -1 ) )
					{
						if( m_figuresOnBoard[hash[ptr].manPtr()].band() != m_figuresOnBoard[m_manPtr].band() )
							hash[ptr].setNote( Position::NOTE_ATTACK);
					}
					/* Attack en Passant Left */
					ptr = calcPointer( -1, 0 );
					if( ( ptr != -1 ) && ( m_enPassant[ 1 - m_figuresOnBoard[m_manPtr].band() ] == ptr ) )
					{
						ptr = calcPointer( -1, dirR );
						hash[ptr].setNote( Position::NOTE_ENPASSANT);
					}
					/* Attack en Passant Right */
					ptr = calcPointer( 1, 0 );
					if( ( ptr != -1 ) && ( m_enPassant[ 1 - m_figuresOnBoard[m_manPtr].band() ] == ptr ) )
					{
						ptr = calcPointer( 1, dirR );
						hash[ptr].setNote( Position::NOTE_ENPASSANT);
					}
				}
				break;
		}
	}

	/***                     THE MONSTER                      ***/
	/*** Remove all possible moves that would either put your ***/
	/*** king into check or wouldn't stop a check in progress ***/

	if( recursion == false )
	{
		hashBackup = QValueVector<Position>(hash);
		chessmanBackup = QValueVector<Figure>(m_figuresOnBoard);
		
		checkNormal();

		m_boardCore = QValueVector<Position>(hashBackup);
		hash = QValueVector<Position>(hashBackup);
		m_figuresOnBoard = QValueVector<Figure>(chessmanBackup);
	}
// 	kdDebug() << "END HASH MOVES*****************" << endl;
}

int KHGame::getKing( const bool band )
{
	for( uint tmp = 0; tmp < 64; tmp++ )
	{
		if( ( m_figuresOnBoard[tmp].band() == band ) && ( m_figuresOnBoard[tmp].type() == Figure::King ) )
		{
			if( isChessman( tmp ) )
			{
				return pointer( m_figuresOnBoard[tmp].file(), m_figuresOnBoard[tmp].rank() );
			}
		}
	}
	return -1;
}

void KHGame::checkNormal()
{
	/* Used for loops and positions */
	int tmp(0), tmp2(0);

	/* Used to calculate a position relative to a given position */
	int dirF(0);

	/* Used to monitor the King inside the Monster */
	int currentKing(0), _castleFlag(0);

	/* Find the King's Position */
	currentKing = getKing( m_figuresOnBoard[m_manPtr].band() );

	/* Remove castles under specific conditions */
	if( _castleFlag )
	{
		for( tmp = 0; tmp < 64; tmp++ )
		{
			if( !isChessman( tmp ) )
				continue;
			if( ( m_figuresOnBoard[tmp].band() != m_figuresOnBoard[m_manPtr].band() ) && ( m_figuresOnBoard[tmp].type() != -1 ) )
			{
				hashMoves( tmp, true );
				/* Is a check in progress? */
				if( hash[ currentKing ].note() == Position::NOTE_ATTACK )
				{
					for( tmp2 = 0; tmp2 < 64; tmp2++ )
						if( hashBackup[tmp2].note() == Position::NOTE_CASTLE )
							hashBackup[tmp2].setNote(Position::NOTE_NONE);
					break;
				}
				else
				{
					/* Store ManPtr in dirF so we can use ManPtr */
					dirF = m_manPtr;
					m_manPtr = hashBackup[ currentKing ].manPtr();
					/* Is the path to Queenside in check? */
					if( _castleFlag & CF_RookQ )
					{
						if( ( hash[ calcPointer( -1, 0 ) ].note() == Position::NOTE_MOVE ) || ( hash[ calcPointer( -2, 0 ) ].note() == Position::NOTE_MOVE ) )
						{
							hashBackup[ calcPointer( -2, 0 ) ].setNote(Position::NOTE_NONE);
							_castleFlag -= CF_RookQ;
						}
					}
					/* Is the path to Kingside in check? */
					if( _castleFlag & CF_RookK )
					{
						if( ( hash[ calcPointer( 1, 0 ) ].note() == Position::NOTE_MOVE ) || ( hash[ calcPointer( 2, 0 ) ].note() == Position::NOTE_MOVE ) )
						{
							hashBackup[ calcPointer( 2, 0 ) ].setNote(Position::NOTE_NONE);
							_castleFlag -= CF_RookK;
						}
					}
					/* Restore ManPtr */
					m_manPtr = dirF;
				}
			}
		}
	} //    <- End Castle Checks

	/* Check all possible moves */
	for( tmp = 0; tmp < 64; tmp++ )
	{
		/* Only proceed if this is a move */
		if( hashBackup[tmp].note() < Position::NOTE_MOVE )
		{
			continue; // This depends on all moves being higher value than NOTE_MOVE,
		}
        // while all non-moves are less.

		/* Pretend we moved here... what would happen? */
		m_boardCore[ pointer( m_figuresOnBoard[m_manPtr].file(), m_figuresOnBoard[m_manPtr].rank() ) ].setManPtr(-1);
		m_figuresOnBoard[m_manPtr].setFile(hashBackup[tmp].file());
		m_figuresOnBoard[m_manPtr].setRank(hashBackup[tmp].rank());
		m_boardCore[tmp].setManPtr(m_manPtr);
		if( m_boardCore[tmp].note() == Position::NOTE_ENPASSANT )
		{
			m_boardCore[ m_enPassant[ 1 - m_figuresOnBoard[m_manPtr].band() ] ].setManPtr(-1);
		}

		/* Recalc King pos, as we may have just moved him */
		currentKing = getKing( m_figuresOnBoard[m_manPtr].band() );

		/* Rehash in new position. If King is now under check, then     */
		/* we can't use this move and it's removed from contention */
		for( tmp2 = 0; tmp2 < 64; tmp2++ )
		{
			if( m_figuresOnBoard[tmp2].band() != m_figuresOnBoard[m_manPtr].band() )
				if( isChessman( tmp2 ) )
			{
				hashMoves( tmp2, true );
				if( hash[ currentKing ].note() == Position::NOTE_ATTACK )
				{
					hashBackup[tmp].setNote(Position::NOTE_NONE);
				}
			}
		}

		/* Restore the playground */
		m_boardCore = QValueVector<Position>(hashBackup);
		m_figuresOnBoard = QValueVector<Figure>(chessmanBackup);

	} //    <- End of 'Check All Moves' loop
}

Figure KHGame::getFigureAt(int boardPos)
{
	if ( boardPos > m_figuresOnBoard.count() )
	{
		kdError() << "Out of index" << endl;
		return Figure();
	}
	return m_figuresOnBoard[ m_boardCore[boardPos].manPtr() ];
}

Position KHGame::getPosition(int boardPos)
{
	return m_boardCore[boardPos];
}

int KHGame::getNoteAt(int boardPos)
{
	return m_boardCore[boardPos].note();
}

QPoint KHGame::getFileAndRank(const KHChessMove &chessMove)
{
	QPoint tmp;
	
	tmp.setX( m_figuresOnBoard[ m_move.manTaken() ].file() );
	tmp.setY( m_figuresOnBoard[ m_move.manTaken() ].rank());
	
	return tmp;
}

void KHGame::setNoteFor(int position, Position::NoteType note)
{
	m_boardCore[ position ].setNote(note);
}

bool KHGame::isChessman( const int figure )
{
	if( figure > 63 )
	{
    		// Always answer TRUE for the Sideboard
		return true;
	}
	if( figure < 0 )
	{
    		// A no-brainer
		return false;
	}

	int boardPtr = pointer( m_figuresOnBoard[figure].file(), m_figuresOnBoard[figure].rank() );
	if( ( boardPtr < 0 ) || ( boardPtr > 63 ) )
	{
		return false;
	}
	if( m_boardCore[boardPtr].manPtr() != figure )
	{
		return false;
	}
	return true;
}

int KHGame::calcPointer( int file, int rank )
{
	int tmpFile, tmpRank;

	tmpFile = m_figuresOnBoard[m_manPtr].file() + file;
	tmpRank = m_figuresOnBoard[m_manPtr].rank() + rank;
	return pointer( tmpFile, tmpRank );
}

bool KHGame::isPaused()
{
	return m_isPaused;
}

bool KHGame::currentBand()
{
	return m_currentBand;
}

void KHGame::setPromotion(const QChar &c)
{
	m_move.setPromote(c);
}

#include "khgame.moc"
