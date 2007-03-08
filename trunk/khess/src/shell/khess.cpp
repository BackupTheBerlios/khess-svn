/***************************************************************************
 *   Copyright (C) 2005-2007 by David Cuadrado - krawek@gmail.com          *
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


#include "khess.h"

#include <QLabel>
#include <QStatusBar>
#include <QMenuBar>

#include <dosd.h>

#include <board/boardview.h>
#include <board/boarditem.h>

#include <game/game.h>

#include "interfaces/engineinterface.h"
#include "interfaces/interfacefactory.h"

#include "gamemanager.h"
#include "matchdialog.h"


struct Khess::Private
{
	GameManager *gameManager;
	
	~Private() 
	{
		delete gameManager;
	}
};

Khess::Khess() : DWorkspaceMainWindow(), d(new Private)
{
	setWindowTitle(tr("Khess"));
	setAcceptDrops(true);
	
	d->gameManager = new GameManager(this);
	
	statusBar()->show();
	setupMenu();
}

Khess::~Khess()
{
	delete d;
}

void Khess::fileOpen()
{
}

void Khess::fileSave()
{
}

void Khess::fileSaveAs()
{
}

void Khess::filePrint()
{
}

void Khess::optionsPreferences()
{
}

void Khess::changeStatusbar(const QString& text)
{
	statusBar()->showMessage(text);
}

void Khess::changeCaption(const QString& text)
{
	setWindowTitle(text);
}

void Khess::setupMenu()
{
	QMenu *file = menuBar()->addMenu(tr("File"));
	
	file->addSeparator();
	file->addAction(tr("Quit"), this, SLOT(close()));
	
	
	QMenu *match = menuBar()->addMenu(tr("Match"));
	match->addAction(tr("New..."), this, SLOT(newMatch()));
}

void Khess::newMatch()
{
	MatchDialog match;
	
	if( match.exec() == QDialog::Accepted )
	{
		Board::BoardView *view = new Board::BoardView;
		addWidget(view);
		
		Game::Game *game = d->gameManager->newGame();
		
		Board::BoardItem *board = view->createBoard(game);
		
		IO::Interface *engine = InterfaceFactory::create<IO::EngineInterface>();
		engine->openResource( match.params() );
		
		connect(board, SIGNAL(moved(const QString &)), engine, SLOT(doMove(const QString &)));
		connect(engine, SIGNAL(moved(const QString &)), board, SLOT(doMove(const QString &)));
	}
}




