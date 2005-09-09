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

#ifndef KHWARRIOR_H
#define KHWARRIOR_H

#include <qmap.h>
#include "khwarriorbase.h"

class KHWarrior;

typedef QMap<QString, QString> PlayerSettings;

/**
 * The chess player
 * @author David Cuadrado <krawek@gmail.com >
*/

class KHWarrior : public KHWarriorBase
{
	Q_OBJECT
	public:
		KHWarrior();
		~KHWarrior();
		
		static KHWarrior *instance();
		
		void setWarriorInformation(const QPixmap &px, const QString &login, const QString &name, const QString &type);
		
		QPixmap face() const;
		void setFicsLogin(const QString &);
		QString ficsLogin();
		void setSettings(const PlayerSettings &settings);
		QString setting(const QString &key);
		
	private:
		QPixmap m_face;
		QString m_ficsLogin;
		PlayerSettings m_settings;

};

#define PLAYER static_cast<KHWarrior *>(KHWarrior::instance())

#endif
