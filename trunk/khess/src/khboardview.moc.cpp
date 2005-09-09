/****************************************************************************
** KHBoardView meta object code from reading C++ file 'khboardview.h'
**
** Created: Tue Aug 16 12:30:07 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "khboardview.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KHBoardView::className() const
{
    return "KHBoardView";
}

QMetaObject *KHBoardView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KHBoardView( "KHBoardView", &KHBoardView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KHBoardView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KHBoardView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KHBoardView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KHBoardView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KHBoardView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = KMdiChildView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "title", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetTitle", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetTitle(const QString&)", &slot_0, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"signalChangeStatusbar", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"signalChangeCaption", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "signalChangeStatusbar(const QString&)", &signal_0, QMetaData::Protected },
	{ "signalChangeCaption(const QString&)", &signal_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"KHBoardView", parentObject,
	slot_tbl, 1,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KHBoardView.setMetaObject( metaObj );
    return metaObj;
}

void* KHBoardView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KHBoardView" ) )
	return this;
    if ( !qstrcmp( clname, "KhessIface" ) )
	return (KhessIface*)this;
    return KMdiChildView::qt_cast( clname );
}

// SIGNAL signalChangeStatusbar
void KHBoardView::signalChangeStatusbar( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL signalChangeCaption
void KHBoardView::signalChangeCaption( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

bool KHBoardView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetTitle((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return KMdiChildView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KHBoardView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: signalChangeStatusbar((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: signalChangeCaption((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return KMdiChildView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool KHBoardView::qt_property( int id, int f, QVariant* v)
{
    return KMdiChildView::qt_property( id, f, v);
}

bool KHBoardView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
