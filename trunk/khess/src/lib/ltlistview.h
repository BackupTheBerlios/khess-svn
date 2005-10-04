/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                        	           *
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

#ifndef LTLISTVIEW_H
#define LTLISTVIEW_H

#include <qvbox.h>
#include <qhbuttongroup.h>
#include <kpushbutton.h>
#include <klistview.h>

#include <ktoolbar.h>
#include <qtimer.h>
#include <kmessagebox.h>
#include <qxml.h>

#include <klistviewsearchline.h>

class KListView;
class LTListView;
class LTListViewItem;
class KLListView;

class ClickLineEdit;

/**
 * Esta clase es la base para crear un widget con una lista y un panel de botones para el manejo de la misma, de esta clase se deben sobreescribir varios metodos, los cuales son:
 * 
 * getClickedItem(): El cual recibe el item de la lista que fue clickeado.
 * 
 * xButtonClicked(): Las acciones que se realizan cuando el boton @b x fue clickeado
 * 
 * y se debe crear el signal que se llame sendWidget(KMdiChildView *), la idea es conectar este signal al slot addModulePage(KMdiChildView *), y pasar el widget construido para ponerlo como vista en la ventana principal.
 * 
 * @short Clase base para listas.
 * @author David Cuadrado
 * @todo 
 * - Agregar una forma de añadir titulo e hijos
*/
class LTListView : public QVBox
{
	Q_OBJECT
	public:
		enum Button { NoButton = 0, ButtonAdd, ButtonDel, ButtonModify, ButtonQuery };
    		
		
		/**
		 * Constructor por defecto
		 */
		LTListView(QWidget *parent = 0, const char *name = 0);
		
		/**
		 * Constructor que recibe como argumentos los botones que se desean. (en orden)
		 */
		LTListView(QStringList colsText, Button button1 = NoButton, Button button2 = NoButton, Button button3 = NoButton, Button button4 = NoButton, QWidget *parent = 0, const char *name = 0);
		~LTListView();
		
		/**
		 * Esta funcion crea los botones
		 */
		void makeButtons(Button button1, Button button2, Button button3, Button button4);
		
		/**
		 * Obtiene el listview
		 */
		KListView *getListView();
		
		/**
		 * Esta funcion crea la lista
		 */
		void makeList(QStringList colsText);
		
//	protected:
		/**
		 * Esta funcion debe implementarse para llenar la lista.
		 */
		virtual void fillList() = 0;
		
	public slots:
		
		/**
		 * Coloca texto al boton especificado
		 */
		void setButtonText(Button b, const QString &text);
		
		/**
		 * Esta funcion nos da el item que fue clickeado
		 */
		virtual void getClickedItem(QListViewItem *item) = 0;
		
		/**
		 * Esta funcion debe sobreescribirse con la accion que se debe ejecutar cuando se presiona el boton de adicionar
		 */
		virtual void addButtonClicked()  = 0;
		
		/**
		 * Esta funcion debe sobreescribirse con la accion que se debe ejecutar cuando se presiona el boton de borrar
		*/
		virtual void delButtonClicked() = 0;
		
		/**
		 * Esta funcion debe sobreescribirse con la accion que se debe ejecutar cuando se presiona el boton de modificar
		*/
		virtual void modifyButtonClicked() = 0;
		
		/**
		 * Esta funcion debe sobreescribirse con la accion que se debe ejecutar cuando se presiona el boton de consultar
		*/
		virtual void queryButtonClicked() = 0;
		
		
		/**
		 * Mete datos a la lista
		 * @param items 
		 */
		virtual void putItems(QStringList items);
		
		/**
		 * Adiciona un item tomando la llave primaria y sacando el registro de la base de datos
		 * @param pkey 
		 */
		virtual void addItem(const QString &pkey); // = 0;
		
	signals:
		
		/**
		 * Emita esto si quiere mostrar una mensaje por medio del OSD.
		 * @param  
		 */
		virtual void message2osd(const QString &); //= 0;
		
		/**
		 * Este signal envia el widget para ser puesto en la ventana principal
		 */
// 		virtual void sendWidget(KMdiChildView *); //= 0;
	
	private:
		QHButtonGroup *m_buttons;
		
		KPushButton *m_buttonAdd;
		KPushButton *m_buttonDel;
		KPushButton *m_buttonModify;
		KPushButton *m_buttonQuery;
		
		ClickLineEdit *m_searchEdit;
		
	protected:
		KLListView *m_listView;

		QXmlInputSource m_xmlsource;
		// Poner el qstring en m_xmlsource con setData, y darle m_xmlreader.parse(m_xmlsource);
		QString m_filterText;
};

class KLListView : public KListView
{
	public:
		KLListView(QWidget *parent=0, const char *name=0);
		~KLListView();
		void viewportPaintEvent( QPaintEvent* );
		void setTitle(const QString &t);
		void setExplain(const QString &exp);
		QString getText(int col);
		
	private:
		QString m_title;
		QString m_explain;
};

class LTListViewItem : public KListViewItem
{
	public:
		LTListViewItem(QListView * parent);
		LTListViewItem(QListViewItem * parent);
		~LTListViewItem();
		int compare ( QListViewItem * i, int col, bool ascending );
};



#endif
