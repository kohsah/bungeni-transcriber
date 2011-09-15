/****************************************************************************
** Meta object code from reading C++ file 'formpost.hpp'
**
** Created: Sun Sep 20 16:06:17 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "formpost.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formpost.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FormPostPlugin[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      18,   16,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FormPostPlugin[] = {
    "FormPostPlugin\0\0r\0readData(QNetworkReply*)\0"
};

const QMetaObject FormPostPlugin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FormPostPlugin,
      qt_meta_data_FormPostPlugin, 0 }
};

const QMetaObject *FormPostPlugin::metaObject() const
{
    return &staticMetaObject;
}

void *FormPostPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FormPostPlugin))
        return static_cast<void*>(const_cast< FormPostPlugin*>(this));
    if (!strcmp(_clname, "FormPost"))
        return static_cast< FormPost*>(const_cast< FormPostPlugin*>(this));
    if (!strcmp(_clname, "com.tuckdesign.Plugin.FormPost/1.0"))
        return static_cast< FormPost*>(const_cast< FormPostPlugin*>(this));
    return QWidget::qt_metacast(_clname);
}

int FormPostPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readData((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
