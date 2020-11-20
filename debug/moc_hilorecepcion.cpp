/****************************************************************************
** Meta object code from reading C++ file 'hilorecepcion.h'
**
** Created: Wed Dec 28 18:52:10 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../hilorecepcion.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hilorecepcion.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HiloRecepcion[] = {

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
      15,   14,   14,   14, 0x05,
      30,   14,   14,   14, 0x05,
      66,   51,   14,   14, 0x05,
     114,  108,   14,   14, 0x05,
     140,   14,   14,   14, 0x05,
     163,   14,   14,   14, 0x05,
     187,  108,   14,   14, 0x05,
     206,  108,   14,   14, 0x05,
     235,  232,   14,   14, 0x05,
     262,   14,   14,   14, 0x05,
     282,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     305,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HiloRecepcion[] = {
    "HiloRecepcion\0\0ReceivedColH()\0"
    "columnasRecibidasH()\0s,c,p,q,provID\0"
    "llegadaVsmallH(double,double,int,int,int)\0"
    "RecID\0signalFillTableCompH(int)\0"
    "signalJabMeth_againH()\0signalInit_VarshaAlgH()\0"
    "signalAllVssH(int)\0signalAllJacobiIEndH(int)\0"
    "id\0signalAllJacobiIIEndH(int)\0"
    "llegadaVectorIntH()\0llegadaVectorDoubleH()\0"
    "decirQueSalio()\0"
};

const QMetaObject HiloRecepcion::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_HiloRecepcion,
      qt_meta_data_HiloRecepcion, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HiloRecepcion::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HiloRecepcion::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HiloRecepcion::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HiloRecepcion))
        return static_cast<void*>(const_cast< HiloRecepcion*>(this));
    return QThread::qt_metacast(_clname);
}

int HiloRecepcion::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ReceivedColH(); break;
        case 1: columnasRecibidasH(); break;
        case 2: llegadaVsmallH((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 3: signalFillTableCompH((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: signalJabMeth_againH(); break;
        case 5: signalInit_VarshaAlgH(); break;
        case 6: signalAllVssH((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: signalAllJacobiIEndH((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: signalAllJacobiIIEndH((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: llegadaVectorIntH(); break;
        case 10: llegadaVectorDoubleH(); break;
        case 11: decirQueSalio(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void HiloRecepcion::ReceivedColH()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void HiloRecepcion::columnasRecibidasH()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void HiloRecepcion::llegadaVsmallH(double _t1, double _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HiloRecepcion::signalFillTableCompH(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void HiloRecepcion::signalJabMeth_againH()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void HiloRecepcion::signalInit_VarshaAlgH()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void HiloRecepcion::signalAllVssH(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void HiloRecepcion::signalAllJacobiIEndH(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void HiloRecepcion::signalAllJacobiIIEndH(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void HiloRecepcion::llegadaVectorIntH()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void HiloRecepcion::llegadaVectorDoubleH()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}
QT_END_MOC_NAMESPACE
