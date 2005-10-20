/***************************************************************************
 *   Copyright (C) 2004 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
  *   Copyright (C) 2004 by David Cuadrado                                  *
 *   krawek@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "button.h"

#include <qpainter.h>
#include <qtooltip.h>
#include <qstyle.h>
#include <qapplication.h>
#include <qtimer.h>

#include <kdebug.h>
#include <kiconloader.h>

#include "buttonbar.h"

namespace Ideal {

	class Button::Animation
	{
		public:
			Animation(QObject *parent) : count(0), MAXCOUNT(20), INTERVAL(30), isEnter(false)
			{
				timer = new QTimer(parent);
			}
			~Animation() {}
			void start()
			{
				timer->start(INTERVAL);
			}
		
			QColor blendColors( const QColor& color1, const QColor& color2, int percent )
			{
				const float factor1 = ( 100 - ( float ) percent ) / 100;
				const float factor2 = ( float ) percent / 100;

				const int r = static_cast<int>( color1.red() * factor1 + color2.red() * factor2 );
				const int g = static_cast<int>( color1.green() * factor1 + color2.green() * factor2 );
				const int b = static_cast<int>( color1.blue() * factor1 + color2.blue() * factor2 );

				QColor result;
				result.setRgb( r % 256, g% 256, b% 256 );

				return result;
			}
			
			QTimer *timer;
			int count;
			const int MAXCOUNT;
			const int INTERVAL;
			
			bool isEnter;
	};
	
	Button::Button(ButtonBar *parent, const QString text, const QIconSet &icon,
		       const QString &description)
	:QPushButton(icon, text, parent), m_buttonBar(parent), m_description(description),
	m_place(parent->place()), m_realText(text), m_realIconSet(icon)
	{
		hide();
		setFlat(true);
		setToggleButton(true);
		setFocusPolicy(NoFocus);
		setDescription(m_description);
		setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		resize(sizeHint());
		fixDimensions(Ideal::Bottom);
    
		QToolTip::add(this, m_realText);
		
		m_animation = new Animation(this);
		
		connect( m_animation->timer, SIGNAL( timeout() ), this, SLOT( animate() ) );
	}

	Button::~Button()
	{
//     m_buttonBar->removeButton(this);
	}

	void Button::setDescription(const QString &description)
	{
		m_description = description;
		QToolTip::remove(this);
		QToolTip::add(this, m_description);
	}

	QString Button::description() const
	{
		return m_description;
	}

	QStyle::SFlags Button::buttonFlags() const
	{
		QStyle::SFlags flags = QStyle::Style_Default;
		if (isEnabled())
			flags |= QStyle::Style_Enabled;
		if (hasFocus())
			flags |= QStyle::Style_HasFocus;
		if (isDown())
			flags |= QStyle::Style_Down;
		if (isOn())
			flags |= QStyle::Style_On;
		if (! isFlat() && ! isDown())
			flags |= QStyle::Style_Raised;
		if (isDefault())
			flags |= QStyle::Style_ButtonDefault;
		
		return flags;
	}
	
	void Button::enterEvent( QEvent* )
	{	
		m_animation->isEnter = true;
		m_animation->count = 1;
	
		m_animation->start();
	}

	void Button::leaveEvent( QEvent* )
	{
		if ( m_animation->count == 0 )
		{
			m_animation->count = 1;
		}
		m_animation->isEnter = false;
		m_animation->start();
	}
	
	void Button::animate()
	{
		if ( m_animation->isEnter ) 
		{
			m_animation->count += 1;
			repaint(false);
			if ( m_animation->count > m_animation->MAXCOUNT )
			{
				m_animation->timer->stop();
			}
		} else 
		{
			m_animation->count -= 1;

			repaint(false);
			if ( m_animation->count < 1 )
			{
				m_animation->timer->stop();
			}
		}
	}
	
	void Button::drawButton(QPainter *p)
	{
		QColor fillColor, textColor;
		
		if ( isOn() || isDown() )
		{
			fillColor = m_animation->blendColors( colorGroup().highlight(), colorGroup().background(), static_cast<int>( m_animation->count * 3.5 ) );
			textColor = m_animation->blendColors( colorGroup().highlightedText(), colorGroup().text(), static_cast<int>( m_animation->count * 4.5 ) );
		}
		else
		{
			fillColor = m_animation->blendColors( colorGroup().background(), colorGroup().highlight(), static_cast<int>( m_animation->count * 3.5 ) );
			textColor = m_animation->blendColors( colorGroup().text(), colorGroup().highlightedText(), static_cast<int>( m_animation->count * 4.5 ) );
		}
		
		QColorGroup cg/* = colorGroup()*/;
		cg.setColor(QColorGroup::Button,  fillColor);
		cg.setColor(QColorGroup::ButtonText, textColor);
		cg.setColor(QColorGroup::Foreground, textColor);
		cg.setColor(QColorGroup::Base, textColor);
		cg.setColor(QColorGroup::Text, textColor);
		cg.setColor(QColorGroup::Mid, textColor);
		
		QRect r = rect();
		QSize sh = r.size()/*.expandedTo(p->fontMetrics().size(Qt::SingleLine,text()))*/;
		
// 		if ( isDown() || isOn() )
// 		{
// 			sh += QSize(6,6);
// 		}
		
		int wsh = sh.width();
		int hsh = sh.height();
		
		switch (m_place)
		{
			case Ideal::Left:
			case Ideal::Right:
				sh.setHeight(wsh);
				sh.setWidth(hsh);
				break;
		}
		
		QPixmap pm(sh.width(), sh.height());
		pm.fill(fillColor);
		
		QPainter p2(&pm);
		
// 		QFont font = p2.font();
// 		font.setBold(isOn() || isDown() );
// 		p2.setFont( font );
		
		QStyle::SFlags flags = buttonFlags();
		
		style().drawControl(QStyle::CE_PushButton,&p2,this, QRect(0,0,pm.width(),pm.height()), cg, flags);
		
		style().drawControl( QStyle::CE_PushButtonLabel, &p2,  this, QRect(0,0,pm.width(),pm.height()), cg, flags, QStyleOption() );
	
		switch (m_place)
		{
			case Ideal::Left:
				p->rotate(-90);
				p->drawPixmap(1-pm.width(), 0, pm);
				break;
			case Ideal::Right:
				p->rotate(90);
				p->drawPixmap(0, 1-pm.height(), pm);
				break;
			default:
				p->drawPixmap(0, 0, pm);
				break;
		}
		
		p2.end();
	}

	void Button::drawButtonLabel(QPainter */*p*/)
	{
	}

	ButtonMode Button::mode()
	{
		return m_buttonBar->mode();
	}

	void Button::setPlace(Ideal::Place place)
	{
		Place oldPlace = m_place;
		m_place = place;
		fixDimensions(oldPlace);
	}

	void Button::fixDimensions(Place oldPlace)
	{
		switch (m_place)
		{
			case Ideal::Left:
			case Ideal::Right:
				if ((oldPlace == Ideal::Bottom) || (oldPlace == Ideal::Top))
				{
					setFixedWidth(height());
					setMinimumHeight(sizeHint().width());
					setMaximumHeight(32767);
				}
				break;
			case Ideal::Top:
			case Ideal::Bottom:
				if ((oldPlace == Ideal::Left) || (oldPlace == Ideal::Right))
				{
					setFixedHeight(width());
					setMinimumWidth(sizeHint().height());
					setMaximumWidth(32767);
				}
				break;
		}
	}

	QSize Button::sizeHint() const
	{
		return sizeHint(text());
	}

	QSize Button::sizeHint(const QString &text) const
	{
		constPolish();
		int w = 0, h = 0;

		if ( iconSet() && !iconSet()->isNull() && (m_buttonBar->mode() != Text) ) 
		{
			int iw = iconSet()->pixmap( QIconSet::Small, QIconSet::Normal ).width() + 4;
			int ih = iconSet()->pixmap( QIconSet::Small, QIconSet::Normal ).height();
			w += iw;
			h = QMAX( h, ih );
		}
		if ( isMenuButton() )
			w += style().pixelMetric(QStyle::PM_MenuButtonIndicator, this);
		if ( pixmap() ) 
		{
			QPixmap *pm = (QPixmap *)pixmap();
			w += pm->width();
			h += pm->height();
		} 
		else if (m_buttonBar->mode() != Icons) 
		{
			QString s( text );
			bool empty = s.isEmpty();
			if ( empty )
				s = QString::fromLatin1("XXXX");
			QFontMetrics fm = fontMetrics();
			QSize sz = fm.size( ShowPrefix, s );
			if(!empty || !w)
				w += sz.width();
			if(!empty || !h)
				h = QMAX(h, sz.height());
		}

		return (style().sizeFromContents(QStyle::CT_ToolButton, this, QSize(w+5, h+5)).
				expandedTo(QApplication::globalStrut()));
	}

	void Ideal::Button::updateSize()
	{
		switch (m_place)
		{
			case Ideal::Left:
			case Ideal::Right:
				setMinimumHeight(sizeHint().width());
				resize(sizeHint().height(), sizeHint().width());
				break;
			case Ideal::Top:
			case Ideal::Bottom:
				resize(sizeHint().width(), sizeHint().height());
				break;
		}
	}

	QString Button::realText() const
	{
		return m_realText;
	}

	void Button::setMode(Ideal::ButtonMode mode)
	{
		switch (mode)
		{
			case Text:
				disableIconSet();
				enableText();
				break;
			case IconsAndText:
				enableIconSet();
				enableText();
				break;
			case Icons:
				disableText();
				enableIconSet();
				break;
		}
		updateSize();
	}

	void Button::enableIconSet()
	{
		if (!iconSet())
		{
			if (m_realIconSet.isNull())
			{
            			m_realIconSet = SmallIcon("file_new");
			}
			setIconSet(m_realIconSet);
		}
	}

	void Button::disableIconSet()
	{
		setIconSet(QIconSet());
	}

	void Button::disableText()
	{
		if (text().length() > 0)
		{
			setText("");
		}
	}

	void Button::enableText()
	{
		setText(m_realText);
	}
}

#include "button.moc"
