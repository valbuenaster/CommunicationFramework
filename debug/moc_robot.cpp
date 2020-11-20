/****************************************************************************
** Meta object code from reading C++ file 'robot.h'
**
** Created: Wed Dec 28 18:52:15 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../robot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'robot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Robot[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x05,
      25,    6,    6,    6, 0x05,
      40,    6,    6,    6, 0x05,
      61,    6,    6,    6, 0x05,
      75,    6,    6,    6, 0x05,
      91,    6,    6,    6, 0x05,
     110,    6,    6,    6, 0x05,
     139,  130,    6,    6, 0x05,
     156,  130,    6,    6, 0x05,
     184,  174,    6,    6, 0x05,
     220,    6,    6,    6, 0x05,
     231,    6,    6,    6, 0x05,
     252,    6,    6,    6, 0x05,
     278,    6,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
     298,    6,    6,    6, 0x08,
     317,    6,    6,    6, 0x08,
     329,    6,    6,    6, 0x08,
     342,    6,    6,    6, 0x08,
     353,    6,    6,    6, 0x08,
     365,    6,    6,    6, 0x08,
     384,    6,    6,    6, 0x08,
     394,    6,    6,    6, 0x08,
     407,    6,    6,    6, 0x08,
     422,    6,    6,    6, 0x08,
     441,  438,    6,    6, 0x08,
     468,  438,    6,    6, 0x08,
     493,  438,    6,    6, 0x08,
     519,    6,    6,    6, 0x08,
     534,    6,    6,    6, 0x08,
     551,    6,    6,    6, 0x08,
     569,    6,    6,    6, 0x08,
     586,    6,    6,    6, 0x08,
     604,    6,    6,    6, 0x08,
     622,    6,    6,    6, 0x08,
     658,  647,    6,    6, 0x08,
     714,  706,    6,    6, 0x08,
     751,  130,    6,    6, 0x08,
     765,  130,    6,    6, 0x08,
     780,    6,    6,    6, 0x08,
     797,    6,    6,    6, 0x08,
     817,    6,    6,    6, 0x08,
     838,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Robot[] = {
    "Robot\0\0ack_GoNextPhase()\0go_NextPhase()\0"
    "signal_MakeSending()\0go_JacobiII()\0"
    "finalJacobiII()\0signalEndJacobiI()\0"
    "signalEndJacobiII()\0columnsP\0"
    "signalsLeft(int)\0signalsRight(int)\0"
    "si,co,p,q\0signalSinCos(double,double,int,int)\0"
    "GotAllVs()\0signalJacobiIagain()\0"
    "signalPosColandENDCycle()\0signalCheckRefere()\0"
    "countingArrivals()\0nextPhase()\0"
    "hacerEnvio()\0SendWord()\0SendSpark()\0"
    "SendSpark_Jacobi()\0SendAck()\0SendPMidJM()\0"
    "arrivalNodes()\0arrivalStates()\0id\0"
    "verificationStageIIJM(int)\0"
    "verificationFinJacI(int)\0"
    "verificationFinJacII(int)\0JacobiMethod()\0"
    "SendEndJacobiI()\0SendEndJacobiII()\0"
    "JacobiMethodII()\0setJacobiOption()\0"
    "setVarshaOption()\0beginSelectedAlgorithm()\0"
    "s,c,p,q,id\0arrivalVsMatrixSiCos(double,double,int,int,int)\0"
    "s,c,p,q\0SendSinCosine(double,double,int,int)\0"
    "SendLeft(int)\0SendRight(int)\0"
    "positionColumn()\0PosColandENDCycle()\0"
    "final_Stage_Refere()\0imprimirMP()\0"
};

const QMetaObject Robot::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Robot,
      qt_meta_data_Robot, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Robot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Robot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Robot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Robot))
        return static_cast<void*>(const_cast< Robot*>(this));
    return QObject::qt_metacast(_clname);
}

int Robot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ack_GoNextPhase(); break;
        case 1: go_NextPhase(); break;
        case 2: signal_MakeSending(); break;
        case 3: go_JacobiII(); break;
        case 4: finalJacobiII(); break;
        case 5: signalEndJacobiI(); break;
        case 6: signalEndJacobiII(); break;
        case 7: signalsLeft((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: signalsRight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: signalSinCos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 10: GotAllVs(); break;
        case 11: signalJacobiIagain(); break;
        case 12: signalPosColandENDCycle(); break;
        case 13: signalCheckRefere(); break;
        case 14: countingArrivals(); break;
        case 15: nextPhase(); break;
        case 16: hacerEnvio(); break;
        case 17: SendWord(); break;
        case 18: SendSpark(); break;
        case 19: SendSpark_Jacobi(); break;
        case 20: SendAck(); break;
        case 21: SendPMidJM(); break;
        case 22: arrivalNodes(); break;
        case 23: arrivalStates(); break;
        case 24: verificationStageIIJM((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: verificationFinJacI((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: verificationFinJacII((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: JacobiMethod(); break;
        case 28: SendEndJacobiI(); break;
        case 29: SendEndJacobiII(); break;
        case 30: JacobiMethodII(); break;
        case 31: setJacobiOption(); break;
        case 32: setVarshaOption(); break;
        case 33: beginSelectedAlgorithm(); break;
        case 34: arrivalVsMatrixSiCos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 35: SendSinCosine((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 36: SendLeft((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: SendRight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: positionColumn(); break;
        case 39: PosColandENDCycle(); break;
        case 40: final_Stage_Refere(); break;
        case 41: imprimirMP(); break;
        default: ;
        }
        _id -= 42;
    }
    return _id;
}

// SIGNAL 0
void Robot::ack_GoNextPhase()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Robot::go_NextPhase()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Robot::signal_MakeSending()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Robot::go_JacobiII()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Robot::finalJacobiII()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void Robot::signalEndJacobiI()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void Robot::signalEndJacobiII()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Robot::signalsLeft(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Robot::signalsRight(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Robot::signalSinCos(double _t1, double _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Robot::GotAllVs()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void Robot::signalJacobiIagain()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void Robot::signalPosColandENDCycle()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void Robot::signalCheckRefere()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}
QT_END_MOC_NAMESPACE
