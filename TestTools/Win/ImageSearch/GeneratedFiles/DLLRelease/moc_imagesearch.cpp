/****************************************************************************
** Meta object code from reading C++ file 'imagesearch.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imagesearch.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagesearch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageSearch[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      49,   42,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ImageSearch[] = {
    "ImageSearch\0\0on_check_foreground_window()\0"
    "reason\0on_icon_activated(QSystemTrayIcon::ActivationReason)\0"
};

void ImageSearch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ImageSearch *_t = static_cast<ImageSearch *>(_o);
        switch (_id) {
        case 0: _t->on_check_foreground_window(); break;
        case 1: _t->on_icon_activated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ImageSearch::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ImageSearch::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ImageSearch,
      qt_meta_data_ImageSearch, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ImageSearch::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ImageSearch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ImageSearch::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageSearch))
        return static_cast<void*>(const_cast< ImageSearch*>(this));
    return QDialog::qt_metacast(_clname);
}

int ImageSearch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE