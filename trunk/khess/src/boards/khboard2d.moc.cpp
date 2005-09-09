/****************************************************************************
** KHBoard2D meta object code from reading C++ file 'khboard2d.h'
**
** Created: Tue Aug 16 12:52:04 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "khboard2d.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KHBoard2D::className() const
{
    return "KHBoard2D";
}

QMetaObject *KHBoard2D::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KHBoard2D( "KHBoard2D", &KHBoard2D::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KHBoard2D::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KHBoard2D", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KHBoard2D::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KHBoard2D", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KHBoard2D::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = KHBoardBase::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pos", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"drawPosition", 1, param_slot_0 };
    static const QUMethod slot_1 = {"drawSprites", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "chessMove", &static_QUType_ptr, "KHChessMove", QUParameter::In }
    };
    static const QUMethod slot_2 = {"drawMove", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "chessMove", &static_QUType_ptr, "KHChessMove", QUParameter::In },
	{ "reverse", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"drawMove", 2, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "board", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setBoard", 1, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "drawPosition(int)", &slot_0, QMetaData::Public },
	{ "drawSprites()", &slot_1, QMetaData::Public },
	{ "drawMove(const KHChessMove&)", &slot_2, QMetaData::Public },
	{ "drawMove(const KHChessMove&,const bool&)", &slot_3, QMetaData::Public },
	{ "setBoard(const QString&)", &slot_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"KHBoard2D", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KHBoard2D.setMetaObject( metaObj );
    return metaObj;
}

void* KHBoard2D::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KHBoard2D" ) )
	return this;
    return KHBoardBase::qt_cast( clname );
}

bool KHBoard2D::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: drawPosition((int)static_QUType_int.get(_o+1)); break;
    case 1: drawSprites(); break;
    case 2: drawMove((const KHChessMove&)*((const KHChessMove*)static_QUType_ptr.get(_o+1))); break;
    case 3: drawMove((const KHChessMove&)*((const KHChessMove*)static_QUType_ptr.get(_o+1)),(const bool&)static_QUType_bool.get(_o+2)); break;
    case 4: setBoard((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return KHBoardBase::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KHBoard2D::qt_emit( int _id, QUObject* _o )
{
    return KHBoardBase::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KHBoard2D::qt_property( int id, int f, QVariant* v)
{
    return KHBoardBase::qt_property( id, f, v);
}

bool KHBoard2D::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
