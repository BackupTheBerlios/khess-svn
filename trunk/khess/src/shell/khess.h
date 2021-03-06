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


#ifndef _KHESS_H_
#define _KHESS_H_

#include "dworkspacemainwindow.h"


/**
 * This class serves as the main window for Khess.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author David Cuadrado - <krawek@gmail.com>
 * @version 0.1
 */

class Khess : public DWorkspaceMainWindow
{
	Q_OBJECT
	public:
		Khess();
		~Khess();
	
	private slots:
		void fileOpen();
		void fileSave();
		void fileSaveAs();
		void filePrint();
		void optionsPreferences();

		void changeStatusbar(const QString& text);
		void changeCaption(const QString& text);
		
		
		void connectToServer();
		void newLocalMatch();
		
		
		
	protected:
		void setupMenu();

	private:
		struct Private;
		Private *const d;
};

#endif // _KHESS_H_

