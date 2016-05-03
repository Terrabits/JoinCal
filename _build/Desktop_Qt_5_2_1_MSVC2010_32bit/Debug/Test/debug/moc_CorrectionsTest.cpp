/****************************************************************************
** Meta object code from reading C++ file 'CorrectionsTest.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../Test/CorrectionsTest.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CorrectionsTest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CorrectionsTest_t {
    QByteArrayData data[7];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CorrectionsTest_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CorrectionsTest_t qt_meta_stringdata_CorrectionsTest = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 15),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 10),
QT_MOC_LITERAL(4, 44, 12),
QT_MOC_LITERAL(5, 57, 12),
QT_MOC_LITERAL(6, 70, 12)
    },
    "CorrectionsTest\0fullCorrections\0\0"
    "subsection\0stopExcluded\0stopIncluded\0"
    "fromCalGroup\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CorrectionsTest[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08,
       3,    0,   40,    2, 0x08,
       4,    0,   41,    2, 0x08,
       5,    0,   42,    2, 0x08,
       6,    0,   43,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CorrectionsTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CorrectionsTest *_t = static_cast<CorrectionsTest *>(_o);
        switch (_id) {
        case 0: _t->fullCorrections(); break;
        case 1: _t->subsection(); break;
        case 2: _t->stopExcluded(); break;
        case 3: _t->stopIncluded(); break;
        case 4: _t->fromCalGroup(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CorrectionsTest::staticMetaObject = {
    { &RsaToolbox::VnaTestClass::staticMetaObject, qt_meta_stringdata_CorrectionsTest.data,
      qt_meta_data_CorrectionsTest,  qt_static_metacall, 0, 0}
};


const QMetaObject *CorrectionsTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CorrectionsTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CorrectionsTest.stringdata))
        return static_cast<void*>(const_cast< CorrectionsTest*>(this));
    return RsaToolbox::VnaTestClass::qt_metacast(_clname);
}

int CorrectionsTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RsaToolbox::VnaTestClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
