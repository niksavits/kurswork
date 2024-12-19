/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Server_t {
    QByteArrayData data[31];
    char stringdata0[420];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Server_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Server_t qt_meta_stringdata_Server = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Server"
QT_MOC_LITERAL(1, 7, 19), // "clientAuthenticated"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "message"
QT_MOC_LITERAL(4, 36, 12), // "handleClient"
QT_MOC_LITERAL(5, 49, 18), // "onAuthenticateUser"
QT_MOC_LITERAL(6, 68, 8), // "username"
QT_MOC_LITERAL(7, 77, 8), // "password"
QT_MOC_LITERAL(8, 86, 11), // "QTcpSocket*"
QT_MOC_LITERAL(9, 98, 12), // "clientSocket"
QT_MOC_LITERAL(10, 111, 19), // "handleSearchRequest"
QT_MOC_LITERAL(11, 131, 6), // "author"
QT_MOC_LITERAL(12, 138, 5), // "title"
QT_MOC_LITERAL(13, 144, 8), // "minPrice"
QT_MOC_LITERAL(14, 153, 8), // "maxPrice"
QT_MOC_LITERAL(15, 162, 7), // "section"
QT_MOC_LITERAL(16, 170, 9), // "publisher"
QT_MOC_LITERAL(17, 180, 8), // "maxPages"
QT_MOC_LITERAL(18, 189, 8), // "minPages"
QT_MOC_LITERAL(19, 198, 7), // "minYear"
QT_MOC_LITERAL(20, 206, 7), // "maxYear"
QT_MOC_LITERAL(21, 214, 24), // "handleGetAllBooksRequest"
QT_MOC_LITERAL(22, 239, 21), // "handleRegisterRequest"
QT_MOC_LITERAL(23, 261, 20), // "handleAddBookRequest"
QT_MOC_LITERAL(24, 282, 8), // "bookData"
QT_MOC_LITERAL(25, 291, 21), // "handleEditBookRequest"
QT_MOC_LITERAL(26, 313, 23), // "handleDeleteBookRequest"
QT_MOC_LITERAL(27, 337, 24), // "approveRequestInDatabase"
QT_MOC_LITERAL(28, 362, 9), // "requestId"
QT_MOC_LITERAL(29, 372, 23), // "rejectRequestInDatabase"
QT_MOC_LITERAL(30, 396, 23) // "deleteRequestInDatabase"

    },
    "Server\0clientAuthenticated\0\0message\0"
    "handleClient\0onAuthenticateUser\0"
    "username\0password\0QTcpSocket*\0"
    "clientSocket\0handleSearchRequest\0"
    "author\0title\0minPrice\0maxPrice\0section\0"
    "publisher\0maxPages\0minPages\0minYear\0"
    "maxYear\0handleGetAllBooksRequest\0"
    "handleRegisterRequest\0handleAddBookRequest\0"
    "bookData\0handleEditBookRequest\0"
    "handleDeleteBookRequest\0"
    "approveRequestInDatabase\0requestId\0"
    "rejectRequestInDatabase\0deleteRequestInDatabase"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Server[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   77,    2, 0x0a /* Public */,
       5,    3,   78,    2, 0x0a /* Public */,
      10,   11,   85,    2, 0x0a /* Public */,
      21,    1,  108,    2, 0x0a /* Public */,
      22,    3,  111,    2, 0x0a /* Public */,
      23,    2,  118,    2, 0x0a /* Public */,
      25,    2,  123,    2, 0x0a /* Public */,
      26,    2,  128,    2, 0x0a /* Public */,
      27,    1,  133,    2, 0x0a /* Public */,
      29,    1,  136,    2, 0x0a /* Public */,
      30,    1,  139,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 8,    6,    7,    9,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    9,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 8,    6,    7,    9,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QJsonObject,    9,   24,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QJsonObject,    9,   24,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QJsonObject,    9,   24,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void, QMetaType::QString,   28,

       0        // eod
};

void Server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Server *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clientAuthenticated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->handleClient(); break;
        case 2: _t->onAuthenticateUser((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QTcpSocket*(*)>(_a[3]))); break;
        case 3: _t->handleSearchRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< const QString(*)>(_a[8])),(*reinterpret_cast< const QString(*)>(_a[9])),(*reinterpret_cast< const QString(*)>(_a[10])),(*reinterpret_cast< const QString(*)>(_a[11]))); break;
        case 4: _t->handleGetAllBooksRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        case 5: _t->handleRegisterRequest((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QTcpSocket*(*)>(_a[3]))); break;
        case 6: _t->handleAddBookRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 7: _t->handleEditBookRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 8: _t->handleDeleteBookRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 9: _t->approveRequestInDatabase((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->rejectRequestInDatabase((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->deleteRequestInDatabase((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Server::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::clientAuthenticated)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Server::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Server.data,
    qt_meta_data_Server,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Server.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Server::clientAuthenticated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
