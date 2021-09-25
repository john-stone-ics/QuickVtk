#pragma once

#include "quickVtkDispatcher.h"
#include "quickVtkObject.h"

#include <QtQml/QQmlListProperty>

#include <QtCore/QList>
#include <QtCore/QDebug>

class vtkRenderWindow;

template<typename MyT,  typename MyVtk,
         typename ObjT, typename ObjVtk,
         void (*Af)(vtkSmartPointer<MyVtk> myVtkObject, vtkSmartPointer<ObjVtk> objVtkObject, int index),
         void (*Rf)(vtkSmartPointer<MyVtk> myVtkObject, vtkSmartPointer<ObjVtk> objVtkObject, int index)>
struct QQmlListProperty_impl
{   
    static void attachObject(MyT* pThis, quick::vtk::Object* object, int index, vtkRenderWindow* renderWindow, typename ObjT::vtkUserData renderData)
    {
        auto dispatcher = pThis->dispatcher();

        if (!dispatcher) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << pThis << "pThis->dispatcher()) FAILED";
            return;
        }

        auto myVtkData = pThis->isVolatile() ? dispatcher->lookup(pThis, renderData) : nullptr;

        if (!myVtkData && pThis->isVolatile()) {
            qWarning() << "YIKES!! dispatcher(pThis, renderData) FAILED";
            return;
        }

        if (myVtkData && !pThis->isVolatile()) {
            qWarning() << "YIKES!! dispatcher->lookup(pThis, renderData) returned data but object is non-volatile";
            return;
        }

        auto myVtkObject = pThis->myVtkObject(myVtkData);

        if (!myVtkObject) {
            qWarning() << "YIKES!!  pThis->myVtkObject(myVtkData) FAILED";
            return;
        }

        auto myDownCastObject = MyVtk::SafeDownCast(myVtkObject);

        if (!myDownCastObject) {
            qWarning() << "YIKES!! MyVtk::SafeDownCast(myVtkObject) FAILED";
            return;
        }

        auto objData = dispatcher->lookup(object, renderData, true);

        if (!objData)
            objData = object->initializeVTK(renderWindow, renderData);

        if (!objData && object->isVolatile()) {
            qWarning() << "YIKES!! object->initializeVTK() FAILED";
            return;
        }

        if (objData && !object->isVolatile()) {
            qWarning() << "YIKES!! dispatcher->lookup(object, renderData) returned data but object is non-volatile";
            return;
        }

        auto objVtkObject = object->myVtkObject(objData);

        if (!objVtkObject) {
            qWarning() << "YIKES!! object->myVtkObject(objData) FAILED";
            return;
        }

        auto objDownCastObject = ObjVtk::SafeDownCast(objVtkObject);

        if (!objDownCastObject) {
            qWarning() << "YIKES!! ObjVtk::SafeDownCast(objVtkObject) FAILED";
            return;
        }

        Af(myDownCastObject, objDownCastObject, index);
    }

    static void detachObjects(MyT* pThis, typename ObjT::vtkUserData renderData, QList<ObjT*> list)
    {
        auto dispatcher = pThis->dispatcher();

        if (!dispatcher) {
            qWarning() << "YIKES!! pThis->m_dispatcher() FAILED";
            return;
        }

        auto myVtkData = dispatcher->lookup(pThis, renderData);

        if (!myVtkData && pThis->isVolatile()) {
            qWarning() << "YIKES!! dispatcher(pThis, renderData) FAILED";
            return;
        }

        if (myVtkData && !pThis->isVolatile()) {
            qWarning() << "YIKES!! dispatcher->lookup(pThis, renderData) returned data but object is non-volatile";
            return;
        }

        auto myVtkObject = pThis->myVtkObject(myVtkData);

        if (!myVtkObject) {
            qWarning() << "YIKES!! pThis->myVtkObject(myVtkData) FAILED";
            return;
        }

        auto myDownCastObject = MyVtk::SafeDownCast(myVtkObject);

        if (!myDownCastObject) {
            qWarning() << "YIKES!! MyVtk::SafeDownCast(myVtkObject) FAILED";
            return;
        }

        for(int index=0; index < list.count(); ++index)
        {
            auto& object = list.at(index);

            auto objData = dispatcher->lookup(object, renderData, !object->isVolatile());

            if (!objData && object->isVolatile()) {
                qWarning() << "YIKES!! object->initializeVTK() FAILED";
                return;
            }

            if (objData && !object->isVolatile()) {
                qWarning() << "YIKES!! dispatcher->lookup(object, renderData) returned data but object is non-volatile";
                return;
            }

            auto objVtkObject = object->myVtkObject(objData);

            if (!objVtkObject) {
                qWarning() << "YIKES!! object->myVtkObject(objUserData) FAILED";
                continue;
            }

            auto objDownCastObject = ObjVtk::SafeDownCast(objVtkObject);

            if (!objDownCastObject) {
                qWarning() << "YIKES!! ObjVtk::SafeDownCast(objVtkObject) FAILED";
                return;
            }

            Rf(myDownCastObject, objDownCastObject, index);

            if (object->isVolatile() && !dispatcher->unmap(object, ObjVtk::SafeDownCast(objVtkObject))) {
                qWarning().nospace() << "YIKES!!   dispatcher->unmap(object, renderData) FAILED";
                continue;
            }
        }
    }

    static void append(QQmlListProperty<ObjT>* l, ObjT* object)
    {
        auto pThis = qobject_cast<MyT*>(l->object);

        if (!pThis) {
            qWarning() << "YIKES!! qobject_cast<MyT*>(l->object) FAILED";
            return;
        }

        auto* list = reinterpret_cast<QList<ObjT*>*>(l->data);

        if (!list) {
            qWarning() << "YIKES!! reinterpret_cast<QList<ObjT*>*>(l->data) FAILED";
            return;
        }

        if (list->contains(object)) {
            qWarning() << "YIKES!! appending an object already in the input list";
            return;
        }

        if (!object) {
            qWarning() << "YIKES!! QML passed us a nullptr for append(.., object), pThis:" << pThis;
            return;
        }

        list->append(object);
        object->addVtkParent(pThis);
        emit pThis->inputChanged();

        if (pThis->m_vtkInitialized)
        {
            pThis->dispatcher()->dispatch_async([
                pThis  = QPointer<MyT>(pThis),
                object = QPointer<ObjT>(object),
                index  = list->count()-1]
            (vtkRenderWindow* renderWindow, typename ObjT::vtkUserData renderData) mutable
            {
                if (!pThis) {
                    qWarning() << "YIKES!! I was deleted";
                    return;
                }

                if (!object) {
                    qWarning() << "YIKES!! object was deleted";
                    return;
                }

                attachObject(pThis, object, index, renderWindow, renderData);
            }, pThis);
        }
    }

    static int count(QQmlListProperty<ObjT>* l)
    {
        auto pThis = qobject_cast<MyT*>(l->object);

        if (!pThis) {
            qWarning() << "YIKES!! qobject_cast<ObjT*>(l->object) FAILED";
            return -1;
        }

        auto* list = reinterpret_cast<QList<ObjT*>*>(l->data);

        if (!list) {
            qWarning() << "YIKES!! reinterpret_cast<QList<ObjT*>*>(l->data) FAILED";
            return -1;
        }

        return list->count();
    }

    static ObjT* at(QQmlListProperty<ObjT>* l, int i)
    {
        auto pThis = qobject_cast<MyT*>(l->object);

        if (!pThis) {
            qWarning() << "YIKES!! qobject_cast<ObjT*>(l->object) FAILED";
            return nullptr;
        }

        auto* list = reinterpret_cast<QList<ObjT*>*>(l->data);

        if (!list) {
            qWarning() << "YIKES!! reinterpret_cast<QList<ObjT*>*>(l->data) FAILED";
            return nullptr;
        }

        if (i<0 || i>=list->count()) {
            qWarning().nospace() << "YIKES!!   i(" << i << ") is out of bounds [" << 0 << "," << list->count() << "]";
            return nullptr;
        }

        return list->at(i);
    }

    static void clear(QQmlListProperty<ObjT>* l)
    {
        auto pThis = qobject_cast<MyT*>(l->object);

        if (!pThis) {
            qWarning() << "YIKES!! qobject_cast<ObjT*>(l->object) FAILED";
            return;
        }

        auto* list = reinterpret_cast<QList<ObjT*>*>(l->data);

        if (!list) {
            qWarning() << "YIKES!! reinterpret_cast<QList<ObjT*>*>(l->data) FAILED";
            return;
        }

        if (pThis->m_vtkInitialized)
        {
            pThis->dispatcher()->dispatch_async([
                pThis = QPointer<MyT>(pThis),
                list = *list] //<-- Note: we make a copy of the list
            (vtkRenderWindow* renderWindow, typename ObjT::vtkUserData renderData) mutable
            {
                if (!pThis) {
                    qWarning() << "YIKES!! I was deleted";
                    return;
                }

                detachObjects(pThis, renderData, list);
            }, pThis);
        }

        for(auto object : *list)
            object->delVtkParent(pThis);
        list->clear();
        emit pThis->inputChanged();
    }
};
