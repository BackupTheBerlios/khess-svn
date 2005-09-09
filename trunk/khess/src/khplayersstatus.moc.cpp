/****************************************************************************
** KHPlayersStatus meta object code from reading C++ file 'khplayersstatus.h'
**
** Created: Tue Aug 16 12:30:02 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "khplayersstatus.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KHPlayersStatus::className() const
{
    return "KHPlayersStatus";
}

QMetaObject *KHPlayersStatus::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KHPlayersStatus( "KHPlayersStatus", &KHPlayersStatus::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KHPlayersStatus::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KHPlayersStatus", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KHPlayersStatus::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KHPlayersStatus", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KHPlayersStatus::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"KHPlayersStatus", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KHPlayersStatus.setMetaObject( metaObj );
    return metaObj;
}

void* KHPlayersStatus::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KHPlayersStatus" ) )
	return this;
    return QVBox::qt_cast( clname );
}

bool KHPlayersStatus::qt_invoke( int _id, QUObject* _o )
{
    return QVBox::qt_invoke(_id,_o);
}

bool KHPlayersStatus::qt_emit( int _id, QUObject* _o )
{
    return QVBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KHPlayersStatus::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool KHPlayersStatus::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
