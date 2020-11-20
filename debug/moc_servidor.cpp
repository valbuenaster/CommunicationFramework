/****************************************************************************
** Meta object code from reading C++ file 'servidor.h'
**
** Created: Wed Dec 28 18:52:20 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../servidor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'servidor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Servidor[] = {

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
      10,    9,    9,    9, 0x05,
      22,    9,    9,    9, 0x05,
      37,    9,    9,    9, 0x05,
      62,    9,    9,    9, 0x05,
      99,   88,    9,    9, 0x05,
     143,    9,    9,    9, 0x05,
     165,    9,    9,    9, 0x05,
     193,  190,    9,    9, 0x05,
     214,  190,    9,    9, 0x05,
     242,  190,    9,    9, 0x05,
     271,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     288,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Servidor[] = {
    "Servidor\0\0makeSpark()\0signalPMidJM()\0"
    "signalJabMeth_againSER()\0"
    "signalInit_VarshaAlgSER()\0s,c,p,q,id\0"
    "llegadaVsmallSER(double,double,int,int,int)\0"
    "llegadaVectorIntSER()\0llegadaVectorDoubleSER()\0"
    "id\0signalAllVssSER(int)\0"
    "signalAllJacobiIEndSER(int)\0"
    "signalAllJacobiIIEndSER(int)\0"
    "ReceivedColSER()\0impMat()\0"
};

const QMetaObject Servidor::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_Servidor,
      qt_meta_data_Servidor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Servidor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Servidor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Servidor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Servidor))
        return static_cast<void*>(const_cast< Servidor*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int Servidor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: makeSpark(); break;
        case 1: signalPMidJM(); break;
        case 2: signalJabMeth_againSER(); break;
        case 3: signalInit_VarshaAlgSER(); break;
        case 4: llegadaVsmallSER((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 5: llegadaVectorIntSER(); break;
        case 6: llegadaVectorDoubleSER(); break;
        case 7: signalAllVssSER((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: signalAllJacobiIEndSER((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: signalAllJacobiIIEndSER((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: ReceivedColSER(); break;
        case 11: impMat(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Servidor::makeSpark()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Servidor::signalPMidJM()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Servidor::signalJabMeth_againSER()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Servidor::signalInit_VarshaAlgSER()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Servidor::llegadaVsmallSER(double _t1, double _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Servidor::llegadaVectorIntSER()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void Servidor::llegadaVectorDoubleSER()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Servidor::signalAllVssSER(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Servidor::signalAllJacobiIEndSER(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Servidor::signalAllJacobiIIEndSER(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Servidor::ReceivedColSER()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}
QT_END_MOC_NAMESPACE
