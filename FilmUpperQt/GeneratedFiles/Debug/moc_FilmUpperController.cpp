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
struct qt_meta_stringdata_Worker_t {
    QByteArrayData data[18];
    char stringdata0[247];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Worker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Worker_t qt_meta_stringdata_Worker = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Worker"
QT_MOC_LITERAL(1, 7, 15), // "secondProcessed"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "second"
QT_MOC_LITERAL(4, 31, 12), // "processEnded"
QT_MOC_LITERAL(5, 44, 18), // "exceptionInProcess"
QT_MOC_LITERAL(6, 63, 15), // "std::exception*"
QT_MOC_LITERAL(7, 79, 1), // "e"
QT_MOC_LITERAL(8, 81, 12), // "startProcess"
QT_MOC_LITERAL(9, 94, 11), // "std::string"
QT_MOC_LITERAL(10, 106, 14), // "fileSourcePath"
QT_MOC_LITERAL(11, 121, 14), // "fileTargetPath"
QT_MOC_LITERAL(12, 136, 21), // "IFrameEnhancerHeader*"
QT_MOC_LITERAL(13, 158, 19), // "frameEnhancerHeader"
QT_MOC_LITERAL(14, 178, 19), // "IFpsEnhancerHeader*"
QT_MOC_LITERAL(15, 198, 17), // "fpsEnhancerHeader"
QT_MOC_LITERAL(16, 216, 16), // "FilmQualityInfo*"
QT_MOC_LITERAL(17, 233, 13) // "targetQuality"

    },
    "Worker\0secondProcessed\0\0second\0"
    "processEnded\0exceptionInProcess\0"
    "std::exception*\0e\0startProcess\0"
    "std::string\0fileSourcePath\0fileTargetPath\0"
    "IFrameEnhancerHeader*\0frameEnhancerHeader\0"
    "IFpsEnhancerHeader*\0fpsEnhancerHeader\0"
    "FilmQualityInfo*\0targetQuality"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Worker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    0,   37,    2, 0x06 /* Public */,
       5,    1,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    5,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Long,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9, 0x80000000 | 12, 0x80000000 | 14, 0x80000000 | 16,   10,   11,   13,   15,   17,

       0        // eod
};

void Worker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Worker *_t = static_cast<Worker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->secondProcessed((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 1: _t->processEnded(); break;
        case 2: _t->exceptionInProcess((*reinterpret_cast< std::exception*(*)>(_a[1]))); break;
        case 3: _t->startProcess((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2])),(*reinterpret_cast< IFrameEnhancerHeader*(*)>(_a[3])),(*reinterpret_cast< IFpsEnhancerHeader*(*)>(_a[4])),(*reinterpret_cast< FilmQualityInfo*(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Worker::*_t)(long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Worker::secondProcessed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Worker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Worker::processEnded)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Worker::*_t)(std::exception * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Worker::exceptionInProcess)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Worker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Worker.data,
      qt_meta_data_Worker,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Worker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Worker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Worker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Worker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Worker::secondProcessed(long _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Worker::processEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Worker::exceptionInProcess(std::exception * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
