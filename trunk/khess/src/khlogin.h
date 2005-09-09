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
#ifndef KHLOGIN_H
#define KHLOGIN_H

#include <qvgroupbox.h>
#include <kdialogbase.h>
#include <klineedit.h>
#include <qlabel.h>

#include "khselectuser.h"

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHLogin : public KDialogBase
{
	Q_OBJECT
	public:
		KHLogin(QWidget *parent = 0 );
		~KHLogin();
		
	private slots:
		void setPlayerInformation();
		void slotOk();
		
	private:
		KHSelectUser *m_selector;
		
		QLabel *m_labelName, *m_labelElo, *m_labelType;
		
		KLineEdit *m_login;
		KLineEdit *m_password;
};

#endif
