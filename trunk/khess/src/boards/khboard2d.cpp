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

#include "khboard2d.h"

#include <kmessagebox.h>
#include <klocale.h>
#include <qpainter.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kio/netaccess.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kpassivepopup.h>

#include "khapp.h"
#include <qstyle.h>
#include <kcursor.h>
#include "kpixmapeffect.h"

#include "khwarrior.h"
#include "khpromotiondialog.h"

KHBoard2D::PrivateSprite::PrivateSprite() : m_restore(false),m_steps(0),m_boardPositionOrigin(0),m_destination(0),m_targetTaken(0)
{
}

KHBoard2D::PrivateSprite::~PrivateSprite()
{
}

void KHBoard2D::PrivateSprite::setFade()
{
	KPixmapEffect::fade(m_sprite, 0.2, Qt::red);
}

 
KHBoard2D::KHBoard2D( QWidget * parent, KHMatch *m, const char * name, WFlags f) : KHBoardBase( m, parent, name ), m_init(false), m_dragSprite(0), m_premoveFrom(-1), m_premoveTo(-1)
{
	Q_CHECK_PTR(m);
	setCursor(KCursor::waitCursor ());
	
	m_drawer.setOptimization( QPixmap::BestOptim );
	m_drawer.resize( 16, 16 );
	m_drawer.fill();

	setMinimumSize( 300, 300 );

	loadFiguresTheme();
	loadBoardTheme();
	
// 	adjustSize();
	m_init = true;
	m_spriteList.setAutoDelete(true);
	
	connect(m_game, SIGNAL(redrawPosition(int)), this, SLOT(drawPosition(int)));
	connect(m_game, SIGNAL(drawMove(const KHChessMove &, const bool &)), this, SLOT(drawMove(const KHChessMove &, const bool &)));
	
	setCursor( QCursor( QPixmap(locate("data", "khess/cursors/hand.png" ) ) ));
// 	adjustSize();
}

KHBoard2D::~KHBoard2D()
{
	if ( m_dragSprite )
	{
		delete m_dragSprite;
	}
}

void KHBoard2D::showPromotionDialog()
{
	KHPromotionDialog promotionDialog(&m_imageManager, m_game->currentBand(), this );
	
	QChar promote = promotionDialog.exec();
	
	if( ( promote != 'q' ) && ( promote != 'b' ) && ( promote != 'n' ) && ( promote != 'k' ) && ( promote != 'r' ) )
	{
		promote = 'q';
	}
	
	m_game->setPromotion(promote);
}

void KHBoard2D::loadBoardTheme(const QString &name )
{
	QString datadir = ::locate("data", "khess/boards/");

	QString boardURL = "tar:" + datadir + "/khess-board.tar.gz/";
	
	QString tempFile = "";
// 	kdDebug() << "FuLLURL: " << boardURL << endl;
	
	if( KIO::NetAccess::download( boardURL + "theme.conf", tempFile, 0 ) )
	{
// 		kdDebug() << "OK URL: " << tempFile  << endl;
	}
	else
	{
// 		kdDebug() << "BAD URL: " << tempFile  << endl;
	}
	
	KSimpleConfig *themeConfig = new KSimpleConfig(tempFile, true);
	
	/* Themes based in Knights ( http://www.knights-chess.com ) */
	themeConfig->setGroup( "General" );
	

	if( themeConfig->readEntry( "Type", "Board" ) == "Board" )
	{
		themeConfig->setGroup( "2DBoard" );

		m_haveBorder = themeConfig->readEntry("HaveBorder", false);
		
		loadThemeItem( KHImageManager::SquareWhite, boardURL + themeConfig->readEntry("Light", "square.light.png") );
		
		
		loadThemeItem( KHImageManager::SquareDark, boardURL + themeConfig->readEntry("Dark", "square.dark.png") );
		
		// 
		loadThemeItem( KHImageManager::Select, boardURL + themeConfig->readEntry("Select", "square.select.png"));
		
		loadThemeItem( KHImageManager::Select, boardURL + themeConfig->readEntry("Motion", "square.motion.png") );
		
		loadThemeItem( KHImageManager::Select, boardURL + themeConfig->readEntry("Danger", "square.danger.png") );

		if ( m_haveBorder )
		{
			loadThemeItem( KHImageManager::Border, boardURL + themeConfig->readEntry("Border", "border.png") );
			
			loadThemeItem( KHImageManager::BorderLightOn, boardURL + themeConfig->readEntry("BorderLightOn", "light_on.png") );
			
			loadThemeItem( KHImageManager::BorderLightOff,  boardURL + themeConfig->readEntry("BorderLightOff", "light_off.png"));
		}
	}

	/* Read coordinate settings */
	themeConfig->setGroup( "Coordinates" );
// 	COLOR_Notation = themeConfig->readColorEntry( "TextColor", &COLOR_White );
// 	COLOR_Notation_Shadow = themeConfig->readColorEntry( "ShadowColor", &COLOR_Black );
	
	
	KIO::NetAccess::removeTempFile( tempFile );
}

void KHBoard2D::loadFiguresTheme(const QString &name ) // TODO: arreglar esta maricada
{
	QString datadir = ::locate("data", "khess/figures/");

	QString figuresURL = "tar:" + datadir + "/khess-figures.tar.gz/";
	
	QString tempFile = "";
// 	kdDebug() << "FuLLURL: " << figuresURL << endl;
	
	if( KIO::NetAccess::download( figuresURL + "theme.conf", tempFile, 0 ) )
	{
// 		kdDebug() << "OK URL: " << tempFile  << endl;
	}
	else
	{
// 		kdDebug() << "BAD URL: " << tempFile  << endl;
	}
	
	KSimpleConfig *themeConfig = new KSimpleConfig(tempFile, true);
	
	/* Themes based in Knights ( http://www.knights-chess.com ) */
	themeConfig->setGroup( "General" );

	if( themeConfig->readEntry( "Type", "Chessmen" ) == "Chessmen" )
	{
		/* Load 2D Chessmen */
		themeConfig->setGroup( "2DBlack" );
		
		loadThemeItem( KHImageManager::BlackKing , figuresURL + themeConfig->readEntry("King", "black.king.png") );
		loadThemeItem( KHImageManager::BlackQueen ,  figuresURL + themeConfig->readEntry("Queen", "black.queen.png"));
		loadThemeItem( KHImageManager::BlackBishop ,  figuresURL + themeConfig->readEntry("Bishop", "black.bishop.png"));
		loadThemeItem( KHImageManager::BlackKnight ,  figuresURL + themeConfig->readEntry("Knight", "black.knight.png") );
		loadThemeItem( KHImageManager::BlackRook ,  figuresURL + themeConfig->readEntry("Rook", "black.rook.png") );
		loadThemeItem( KHImageManager::BlackPawn ,  figuresURL + themeConfig->readEntry("Pawn", "black.pawn.png"));
		
		themeConfig->setGroup( "2DWhite" );
		loadThemeItem( KHImageManager::WhiteKing ,  figuresURL + themeConfig->readEntry("King", "white.king.png"));
		loadThemeItem( KHImageManager::WhiteQueen ,  figuresURL + themeConfig->readEntry("Queen", "white.queen.png") );
		loadThemeItem( KHImageManager::WhiteBishop ,  figuresURL + themeConfig->readEntry("Bishop", "white.bishop.png") );
		loadThemeItem( KHImageManager::WhiteKnight ,  figuresURL + themeConfig->readEntry("Knight", "white.knight.png") );
		loadThemeItem( KHImageManager::WhiteRook ,  figuresURL + themeConfig->readEntry("Rook", "white.rook.png"));
		loadThemeItem( KHImageManager::WhitePawn ,  figuresURL + themeConfig->readEntry("Pawn", "white.pawn.png"));
	}
	
	KIO::NetAccess::removeTempFile( tempFile );
}

void KHBoard2D::loadThemeItem( KHImageManager::Images imageIndex, const QString &URL )
{
	QString tempFile;

// 	kdDebug() << "LOAD: " << URL << endl;
	if( KIO::NetAccess::download( URL, tempFile, NULL ) )
	{
		QImage image;

		if ( image.load(tempFile) )
		{
			if ( m_imageManager.add(imageIndex, image ) )
			{
// 				kdDebug() << "PUDO INSERTAR " << URL << endl;
			}
			else
			{
// 				kdDebug() << "NO PUDO INSERTAR " << URL << endl;
			}
		}
		else
		{
			kdError() << "KHBoard2D::loadThemeItem: Can't read image " << URL << endl;
		}
		
		KIO::NetAccess::removeTempFile( tempFile );
	}
	else
	{
		kdError() << "KHBoard2D::loadThemeItem: Can not extract " << URL << endl;
	}
	return;
}

void KHBoard2D::paintEvent(QPaintEvent * e)
{
// 	kdDebug() << "PAINT EVENT" << endl;
	
	if ( ! m_init )
	{
		return;
	}
	
	QRegion reg = e->region();

	QPainter painter( this );
	painter.setClipRegion( reg );
	painter.drawPixmap( 0, 0, m_drawer );

	painter.end();
}

void KHBoard2D::resizeEvent( QResizeEvent * e )
{
// 	kdDebug() << "RESIZE" << endl;
	
	if ( ! m_init )
	{
		return;
	}

	int tmp(0);
	int statusX(0);
	int size = 9;
	int margin = QApplication::style().pixelMetric( QStyle::PM_DefaultFrameWidth );

	/* Get new size for each square on the board */
	int newWidth = width() / size;
	
	int newHeight = ( height() - margin ) / size;
	
	if( newWidth < newHeight )
	{
		m_squareSize = newWidth;
	}
	else
	{
		m_squareSize = newHeight;
	}

	if( ( m_squareSize < IMAGE_MIN ) || ( m_squareSize > IMAGE_MAX ) /*|| ( m_init == true )*/ )
	{
		kdDebug() << "KHBoard2D::resizeEvent: Image out of range "<< m_squareSize << endl;
		return;
	}
	
	setCursor( QCursor( QImage(locate("data", "khess/cursors/hand.png" ) ).smoothScale(m_squareSize/2, m_squareSize/2) ));
	
	m_boardSize = m_squareSize * size;
	m_sideSquareSize = m_boardSize / 10;

	m_drawer.resize( width(), height() );

	m_rectBoard.setRect( ( width() - m_boardSize ) / 2, ( height() - m_boardSize - margin ) / 2, m_boardSize, m_boardSize );

	/* Now paint the background */

	QPainter painter( &m_drawer );
		
	painter.setBackgroundColor( paletteBackgroundColor() );
	painter.eraseRect( m_drawer.rect() );

	/* Finished Painting */
	painter.end();

	redrawAll();
}

void KHBoard2D::redrawAll()
{
	if( ( m_squareSize < IMAGE_MIN ) || ( m_squareSize > IMAGE_MAX ) )
		return;

	QPixmap buffer(m_imageManager.getWithFade( KHImageManager::Border, m_squareSize, 0.2, Qt::cyan ) );
	
	kdDebug() << "Border size: " << buffer.width() << " x " << buffer.height() << endl;

	bitBlt( &m_drawer, m_rectBoard.x(), m_rectBoard.y(), &buffer, 0, 0, buffer.width(), buffer.height(), Qt::CopyROP );

	/* Redraw All Positions */
	int tmp = 0;

	while( tmp < 64 )
	{
		drawPosition( tmp++ );
	}
	
	repaint( false );
	
}

void KHBoard2D::drawPosition( int pos )
{
	int rank = pos >> 3;
	int file = pos % 8;
	QPixmap buffer;

	if( pos < 0 || pos > 63  )
	{
		return;
	}

	Position position = m_game->getPosition( pos );

	//Traditional board
	if( abs( ( rank % 2 ) - ( file % 2 ) ) )
	{
		buffer = m_imageManager.getWithFade( KHImageManager::SquareWhite , m_squareSize, 0.2, Qt::cyan );
	}
	else
	{
		buffer = m_imageManager.getWithFade( KHImageManager::SquareDark,m_squareSize, 0.2, Qt::green);
	}

	if ( ! m_game->isPaused() )
	{
		QPixmap temp;
		
		switch ( position.note() )
		{
			case Position::NOTE_SELECT:
			{
				QPixmap temp = m_imageManager.get(KHImageManager::Select, m_squareSize);
			}
			break;
			
			case Position::NOTE_HIGHLIGHT:
			{
			}
			break;
			
		}
		
		bitBlt( &buffer, 0, 0, &temp, 0, 0, -1, -1, Qt::CopyROP, false);
	}
	
	if ( !isSprite(pos) )
	{
		if( ( pos < 64 ) || (m_game->getFigureAt(pos).file() > 0 ) )
		{
			QPixmap chessman = getFigure( pos );
			bitBlt( &buffer, 0, 0, &chessman, 0, 0, -1, -1, Qt::CopyROP, false); // DIBUJA FIGURAS
		}
	}

// 	if( pos > 63 )
// 	{
// 		/* Draw Piece Count */
// 		drawCount( &buffer, m_game->chessman[ thePosition.ManPtr ].File );
// 	}

	/* Draw to the main board */
	myBlit( coords( rank, file ), &buffer, buffer.rect() );
}

void KHBoard2D::drawMove( const KHChessMove &chessMove, const bool &reverse )
{
	kdDebug() << "DRAW MOVE" << endl;
	
	int fromPtr, toPtr, takenPtr(-1);
	if( reverse )
	{
		fromPtr = ( chessMove.toRank() << 3 ) + chessMove.toFile();
		toPtr = ( chessMove.fromRank() << 3 ) + chessMove.fromFile();
	}
	else
	{
		fromPtr = ( chessMove.fromRank() << 3 ) + chessMove.fromFile();
		toPtr = ( chessMove.toRank() << 3 ) + chessMove.toFile();
	}

	/* Position where Man was taken != Target Position; ie. en passant */
	if( chessMove.manTaken() != -1 )
	{
		QPoint posTmp = m_game->getFileAndRank(chessMove);
		takenPtr = m_game->pointer( posTmp.x(), posTmp.y() );
	}
	else if( chessMove.san().contains( "x" ) )
	{
		takenPtr = toPtr;
	}

	/* Show Highlights */

	m_game->setNoteFor(fromPtr, Position::NOTE_MOVE);
	if( takenPtr != -1 )
	{
		m_game->setNoteFor(toPtr, Position::NOTE_ATTACK);
	}
	else
	{
		m_game->setNoteFor(toPtr, Position::NOTE_MOVE);
	}

	/* Show Animation */
// 	if( myResource->OPTION_Animate_Moves && !lastMoveWasDrag && isVisible() )
// 	{
// 		QTimer::singleShot( 1, this, SLOT( updateVisuals() ) );
// 		Sprite *spritePtr = new Sprite;
// 		spritePtr->Steps = 1;
// 		spritePtr->Restore = false;
// 		spritePtr->POSITION_Origin = fromPtr;
// 		spritePtr->POSITION_Destination = toPtr;
// 		spritePtr->POSITION_TargetTaken = takenPtr;
// 		if( !reverse )
// 		{
// 			spritePtr->POINT_Origin = coords( chessMove.fromRank, chessMove.fromFile );
// 			spritePtr->POINT_Destination = coords( chessMove.toRank, chessMove.toFile );
// 			spritePtr->PIXMAP_Sprite = getChessman( spritePtr->POSITION_Destination );
// 		}
// 		else
// 		{
// 			spritePtr->POINT_Origin = coords( chessMove.toRank, chessMove.toFile );
// 			spritePtr->POINT_Destination = coords( chessMove.fromRank, chessMove.fromFile );
// 			spritePtr->PIXMAP_Sprite = getChessman( spritePtr->POSITION_Origin );
// 		}
// 		spritePtr->POINT_Current = spritePtr->POINT_Origin;
// 		spritePtr->PIXMAP_FlipFrame.resize( spritePtr->PIXMAP_Sprite.size() );
// 		sprites.append( spritePtr );
// 	}
// 	else
	{
		/* Draw this position only if we're not animating */
		drawPosition( toPtr);
// 		drawVisuals( toPtr, HighlightFadeUp );
		if( takenPtr != -1 )
		{
			drawPosition(takenPtr);
// 			drawVisuals( takenPtr, HighlightFadeUp );
		}
	}

	/* Draw the originating position */
// 	drawVisuals( fromPtr, HighlightFadeUp );

	drawPosition(fromPtr);
// 	if( QString( chessMove.SAN ).contains( "o-o", false ) )
// 	{
// 		/* This is a castle */
// 		ChessMove newMove;
// 		strcat( newMove.SAN, QString( "no" ).latin1() );
// 		newMove.fromRank = chessMove.fromRank;
// 		newMove.toRank = chessMove.toRank;
// 		newMove.ManTaken = -1;
// 		if( QString( chessMove.SAN ).contains( "o-o-o", false ) )
// 		{
// 			/* Queenside */
// 			newMove.fromFile = 0;
// 			newMove.toFile = 3;
// 		}
// 		else
// 		{
// 			/* Kingside */
// 			newMove.fromFile = 7;
// 			newMove.toFile = 5;
// 		}
// 		drawMove( newMove, reverse );
// 	}
// 	lastMoveWasDrag = false;
}


bool KHBoard2D::isSprite( const int &pos )
{
// 	kdDebug() << "Sprite count: " << m_spriteList.count() << endl;
	for( uint index = 0; index < m_spriteList.count(); index++ )
	{
		PrivateSprite *tmpSprite = m_spriteList.at( index );
		if( tmpSprite->m_boardPositionOrigin == pos )
		{
			return true;
		}
	}
	return false;
}

QPoint KHBoard2D::coords( const int &rank, const int &file )
{
	QPoint tmp;

	if( rank < 8 )
	{
		/* Standard Position */
		if( orientation() )
		{
			tmp.setX( m_squareSize * file );
			tmp.setY( m_squareSize * ( 7 - rank ) );
		}
		else
		{
			tmp.setX( m_squareSize * ( 7 - file ) );
			tmp.setY( m_squareSize * rank );
		}
		
		if( m_haveBorder )
		{
			tmp.setX( tmp.x() + ( m_squareSize >> 1 ) );
			tmp.setY( tmp.y() + ( m_squareSize >> 1 ) );
		}
		tmp += m_rectBoard.topLeft();
	}
	
	return tmp;
}

QPixmap KHBoard2D::getFigure(const int &pos)
{
	QPixmap temp;

	int type;
	bool band;
	Figure figTmp;

	if( pos == m_premoveTo )
	{
		figTmp = m_game->getFigureAt(m_premoveFrom);
	}
	else
	{
		figTmp = m_game->getFigureAt(pos);
	}
	
	type = figTmp.type();
	band = figTmp.band();
	
// 	kdDebug() << "Type: " << type << endl;

	switch( type )
	{
		case Figure::King:
		{
			if ( band )
			{
				temp = m_imageManager.get(KHImageManager::WhiteKing, m_squareSize);
			}
			else
			{
				temp = m_imageManager.get(KHImageManager::BlackKing, m_squareSize);
			}
			break;
		}
		case Figure::Queen:
		{
			if ( band )
			{
				temp = m_imageManager.get(KHImageManager::WhiteQueen, m_squareSize);
			}
			else
			{
				temp = m_imageManager.get(KHImageManager::BlackQueen, m_squareSize);
			}
			break;
		}
		case Figure::Bishop:
		{
			if ( band )
			{
				temp = m_imageManager.get(KHImageManager::WhiteBishop, m_squareSize);
			}
			else
			{
				temp = m_imageManager.get(KHImageManager::BlackBishop, m_squareSize);
			}
			break;
		}
		case Figure::Knight:
		{
			if ( band )
			{
				temp = m_imageManager.get(KHImageManager::WhiteKnight, m_squareSize);
			}
			else
			{
				temp = m_imageManager.get(KHImageManager::BlackKnight, m_squareSize);
			}
			break;
		}
		case Figure::Rook:
		{
			if ( band )
			{
				temp = m_imageManager.get(KHImageManager::WhiteRook, m_squareSize);
			}
			else
			{
				temp = m_imageManager.get(KHImageManager::BlackRook, m_squareSize);
			}
			break;
		}
		case Figure::Pawn:
		{
			if ( band )
			{
				temp = m_imageManager.get(KHImageManager::WhitePawn, m_squareSize);
			}
			else
			{
				temp = m_imageManager.get(KHImageManager::BlackPawn, m_squareSize);
			}
			break;
		}
		default:
		{
			break;
		}
	}

	return temp;
}

void KHBoard2D::myBlit( const QPoint &dp, const QPaintDevice *src, const QRect &sr )
{
	bitBlt( &m_drawer, dp.x(), dp.y(), src, sr.x(), sr.y(), sr.width(), sr.height(), Qt::CopyROP );
	update( QRect( dp.x(), dp.y(), sr.width(), sr.height() ) );
}


// Movement

int KHBoard2D::position( const QPoint &aPoint )
{
	QPoint point( aPoint );

	if( m_rectBoard.contains( point ) )
	{
		int file, rank;
		point -= m_rectBoard.topLeft();
		if( m_haveBorder )
		{
			point.setX( point.x() - ( m_squareSize >> 1 ) );
			point.setY( point.y() - ( m_squareSize >> 1 ) );
			if( ( point.x() < 0 ) || ( point.y() < 0 ) || ( point.x() > m_squareSize * 8 ) || ( point.y() > m_squareSize * 8 ) )
			{
				return -1;
			}
		}
		if( orientation() )
		{
			file = point.x() / m_squareSize;
			rank = 7 - ( point.y() / m_squareSize );
		}
		else
		{
			file = 7 - ( point.x() / m_squareSize );
			rank = point.y() / m_squareSize;
		}
		return ( ( rank << 3 ) + file );
	}

	return -1;
}


void KHBoard2D::mouseReleaseEvent( QMouseEvent *event )
{
// 	kdDebug() << "Begin mouse release" << endl;
	event->accept();
		
	if( m_dragSprite )
	{
		int tmp = m_dragSprite->m_boardPositionOrigin;
		
		myBlit( m_dragSprite->m_lastPoint,
			&m_dragSprite->m_flipFrame,
			m_dragSprite->m_flipFrame.rect() );
		m_spriteList.removeRef( m_dragSprite );
		m_dragSprite = 0;
		drawPosition( tmp );
	}
	
	m_game->slotSelect ( position( event->pos() ) );
	
// 	kdDebug() << "End mouse release" << endl;
}

void KHBoard2D::mousePressEvent( QMouseEvent *event )
{
// 	kdDebug() << "Begin mouse press" << endl;
	m_pressPoint = event->pos();
	event->accept();
// 	kdDebug() << "End mouse press" << endl;
}

void KHBoard2D::mouseMoveEvent( QMouseEvent *event )
{
// 	kdDebug() << "Begin mouse move" << endl;
	QPoint currentPos = event->pos();
	if( ! m_dragSprite )
	{
		if( event->state() & Qt::LeftButton )
		{
			if( abs( m_pressPoint.x() - currentPos.x() ) + abs( m_pressPoint.y() - currentPos.y() ) > 6 )
			{
				/* Begin Dragging a piece */
				m_dragSprite = new PrivateSprite;
				
				m_spriteList.append( m_dragSprite );
				
				m_dragSprite->m_originPoint = m_pressPoint;
				m_dragSprite->m_boardPositionOrigin = position( m_pressPoint );
				
				m_game->slotSelect( m_dragSprite->m_boardPositionOrigin );
				
				if( m_game->getNoteAt(m_dragSprite->m_boardPositionOrigin) != Position::NOTE_SELECT )
				{
					/* The selection didn't take.. back out. */
					m_spriteList.removeRef( m_dragSprite );
					m_dragSprite = 0;
					drawPosition( position( m_pressPoint ) );
					return;
				}
				

				/* Get the piece image and store it in dragPix */
				m_dragSprite->m_sprite = getFigure( m_dragSprite->m_boardPositionOrigin );
				m_dragSprite->setFade();
				m_dragSprite->m_flipFrame.resize( m_dragSprite->m_sprite.size() );
				m_dragSprite->m_restore = false;
			}
			else
			{
				/* Not enough dragging */
				return;
			}
		} // End ( event->state() & Qt::LeftButton )
		else
		{
			return; /* No dragging. Most events should end up here */
		}
	}

	int halfSize = m_squareSize >> 1;
	m_dragSprite->m_currentPoint.setX( currentPos.x() - halfSize );
	m_dragSprite->m_currentPoint.setY( currentPos.y() - halfSize );
	
	drawSprites();	
	
// 	kdDebug() << "End mouse move" << endl;
}

void KHBoard2D::wheelEvent( QWheelEvent *event )
{
	kdDebug() << "Wheel!!" << endl;
	event->accept();
}

void KHBoard2D::drawSprites( void )
{
	int index;
	PrivateSprite *spritePtr;

	/* Remove all m_spriteList from the pixmap in reverse order */
	QPainter painter( &m_drawer );

	for( index = (signed int)m_spriteList.count() - 1; index > -1; index-- )
	{
		spritePtr = m_spriteList.at(index);
		if( spritePtr->m_restore == true )
		{
			painter.drawPixmap( spritePtr->m_lastPoint, spritePtr->m_flipFrame );
			update( QRect( spritePtr->m_lastPoint, spritePtr->m_flipFrame.size() ) );
		}
	}
	/* Redraw all m_spriteList */
	for( index = 0; index < (signed int)m_spriteList.count(); index++ )
	{
		spritePtr = m_spriteList.at(index);

		if( ( spritePtr == m_dragSprite ) /*|| ( spritePtr->m_steps < MAX_STEPS )*/ )
		{
			/* Obtain next flip frame */
			bitBlt( &spritePtr->m_flipFrame,
				 0,
				 0,
				 &m_drawer,
				 spritePtr->m_currentPoint.x(),
				 spritePtr->m_currentPoint.y(),
				 spritePtr->m_flipFrame.width(),
				 spritePtr->m_flipFrame.height(),
				 Qt::CopyROP );
			if( spritePtr->m_restore == false )
			{
				spritePtr->m_restore = true;
			}
			/* Redraw Sprite */
			painter.drawPixmap( spritePtr->m_currentPoint, spritePtr->m_sprite );
			update( QRect( spritePtr->m_currentPoint, spritePtr->m_sprite.size() ) );
			spritePtr->m_lastPoint = spritePtr->m_currentPoint;
		}
		else
		{
			/* Animation finished */
			int origin = spritePtr->m_boardPositionOrigin;
			int destination = spritePtr->m_destination;
			int target = spritePtr->m_targetTaken;
			m_spriteList.removeRef( spritePtr );
			drawPosition( origin );
// 			drawVisuals( destination, HighlightFadeUp );
			if( destination != target )
			{
// 				drawVisuals( target, HighlightFadeUp );
			}
			index = -1;
		}
	}
	painter.end();
}

void KHBoard2D::setBoard(const QString &board)
{
	m_game->setBoard(board);
}

void KHBoard2D::doMove(KHChessMove *move)
{
	m_game->doMove(move);
}

KHGame *KHBoard2D::game()
{
	return m_game;
}


#include "khboard2d.moc"
