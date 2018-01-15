/****************************************************************************
** Meta object code from reading C++ file 'FilmUpperController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../FilmUpperController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FilmUpperController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FilmUpperController_t {
    QByteArrayData data[8];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FilmUpperController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FilmUpperController_t qt_meta_stringdata_FilmUpperController = {
    {
QT_MOC_LITERAL(0, 0, 19), // "FilmUpperController"
QT_MOC_LITERAL(1, 20, 15), // "secondProcessed"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 6), // "second"
QT_MOC_LITERAL(4, 44, 12), // "processEnded"
QT_MOC_LITERAL(5, 57, 18), // "exceptionInProcess"
QT_MOC_LITERAL(6, 76, 15), // "std::exception*"
QT_MOC_LITERAL(7, 92, 1) // "e"

    },
    "FilmUpperController\0secondProcessed\0"
    "\0second\0processEnded\0exceptionInProcess\0"
    "std::exception*\0e"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FilmUpperController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    0,   32,    2, 0x06 /* Public */,
       5,    1,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Long,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void FilmUpperController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FilmUpperController *_t = static_cast<FilmUpperController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->secondProcessed((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 1: _t->processEnded(); break;
        case 2: _t->exceptionInProcess((*reinterpret_cast< std::exception*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FilmUpperController::*_t)(long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FilmUpperController::secondProcessed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FilmUpperController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FilmUpperController::processEnded)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (FilmUpperController::*_t)(std::exception * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FilmUpperController::exceptionInProcess)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject FilmUpperController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FilmUpperController.data,
      qt_meta_data_FilmUpperController,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FilmUpperController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FilmUpperController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FilmUpperController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FilmUpperController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FilmUpperController::secondProcessed(long _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FilmUpperController::processEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FilmUpperController::exceptionInProcess(std::exception * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
