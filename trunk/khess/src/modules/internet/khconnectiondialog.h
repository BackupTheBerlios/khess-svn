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

#ifndef KHCONNECTIONDIALOG_H
#define KHCONNECTIONDIALOG_H

#include <kdialogbase.h>
#include <klineedit.h>
#include <qlabel.h>
#include <qvgroupbox.h>
#include <qcheckbox.h>

/**
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHConnectionDialog : public KDialogBase
{
	Q_OBJECT
	public:
		enum { Register = 69 };
		
		KHConnectionDialog(QWidget *parent = 0);
		~KHConnectionDialog();
		
		QString login();
		QString password();
		QString server();
		QString port();
		
		bool asGuest();
		
	private slots:
		void verifyGuest(int);
		
	protected slots:
		virtual void slotOk();
		virtual void slotUser1 ();
		
	private:
		KLineEdit *m_loginLE, *m_passwordLE, *m_serverLE, *m_portLE;
		QCheckBox *m_guest;
		QString m_login;
};

#endif
