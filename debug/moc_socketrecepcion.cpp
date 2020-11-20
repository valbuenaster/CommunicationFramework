/****************************************************************************
** Meta object code from reading C++ file 'socketrecepcion.h'
**
** Created: Wed Dec 28 18:52:25 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../socketrecepcion.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'socketrecepcion.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SocketRecepcion[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      31,   16,   16,   16, 0x05,
      66,   51,   16,   16, 0x05,
     107,   16,   16,   16, 0x05,
     126,   16,   16,   16, 0x05,
     154,  148,   16,   16, 0x05,
     179,   16,   16,   16, 0x05,
     201,  148,   16,   16, 0x05,
     219,  148,   16,   16, 0x05,
     244,  148,   16,   16, 0x05,
     270,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     293,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SocketRecepcion[] = {
    "SocketRecepcion\0\0ReceivedCol()\0"
    "columnasRecibidas()\0s,c,p,q,provID\0"
    "llegadaVsmall(double,double,int,int,int)\0"
    "llegadaVectorInt()\0llegadaVectorDouble()\0"
    "RecID\0signalFillTableComp(int)\0"
    "signalJabMeth_again()\0signalAllVss(int)\0"
    "signalAllJacobiIEnd(int)\0"
    "signalAllJacobiIIEnd(int)\0"
    "signalInit_VarshaAlg()\0LeerCliente()\0"
};

const QMetaObject SocketRecepcion::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_SocketRecepcion,
      qt_meta_data_SocketRecepcion, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SocketRecepcion::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SocketRecepcion::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SocketRecepcion::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SocketRecepcion))
        return static_cast<void*>(const_cast< SocketRecepcion*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int SocketRecepcion::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ReceivedCol(); break;
        case 1: columnasRecibidas(); break;
        case 2: llegadaVsmall((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 3: llegadaVectorInt(); break;
        case 4: llegadaVectorDouble(); break;
        case 5: signalFillTableComp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: signalJabMeth_again(); break;
        case 7: signalAllVss((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: signalAllJacobiIEnd((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: signalAllJacobiIIEnd((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: signalInit_VarshaAlg(); break;
        case 11: LeerCliente(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SocketRecepcion::ReceivedCol()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SocketRecepcion::columnasRecibidas()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SocketRecepcion::llegadaVsmall(double _t1, double _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SocketRecepcion::llegadaVectorInt()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void SocketRecepcion::llegadaVectorDouble()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void SocketRecepcion::signalFillTableComp(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SocketRecepcion::signalJabMeth_again()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void SocketRecepcion::signalAllVss(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void SocketRecepcion::signalAllJacobiIEnd(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void SocketRecepcion::signalAllJacobiIIEnd(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void SocketRecepcion::signalInit_VarshaAlg()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}
QT_END_MOC_NAMESPACE
