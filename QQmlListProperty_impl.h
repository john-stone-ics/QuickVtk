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
        auto dispatcher = pThis->m_weakDispatcher.lock();

        if (!dispatcher) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << pThis << "pThis->m_weakDispatcher.lock() FAILED";
            return;
        }

        auto myVtkData = dispatcher->lookup(pThis, renderData);

        if (!myVtkData) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "dispatcher(pThis, renderData) FAILED";
            return;
        }

        auto myVtkObject = pThis->myVtkObject(myVtkData);

        if (!myVtkObject) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << " pThis->myVtkObject(myVtkData) FAILED";
            return;
        }

        auto myDownCastObject = MyVtk::SafeDownCast(myVtkObject);

        if (!myDownCastObject) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "MyVtk::SafeDownCast(myVtkObject) FAILED";
            return;
        }

        auto objData = dispatcher->lookup(object, renderData, true);

        if (!objData)
            objData = object->initializeVTK(pThis->m_weakDispatcher, renderWindow, renderData);

        if (!objData && object->isVolatile()) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "object->initializeVTK() FAILED";
            return;
        }

        if (objData && !object->isVolatile()) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "object->myVtkObject(objData) returned data but object is non-volatile";
            return;
        }

        auto objVtkObject = object->myVtkObject(objData);

        if (!objVtkObject) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "object->myVtkObject(objData) FAILED";
            return;
        }

        auto objDownCastObject = ObjVtk::SafeDownCast(objVtkObject);

        if (!objDownCastObject) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "ObjVtk::SafeDownCast(objVtkObject) FAILED";
            return;
        }

        Af(myDownCastObject, objDownCastObject, index);
    }

    static void detachObjects(MyT* pThis, typename ObjT::vtkUserData renderData, QList<ObjT*> list)
    {
        auto dispatcher = pThis->m_weakDispatcher.lock();

        if (!dispatcher) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << pThis << "pThis->m_weakDispatcher.lock() FAILED";
            return;
        }

        auto myVtkData = dispatcher->lookup(pThis, renderData);

        if (!myVtkData) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "dispatcher(pThis, renderData) FAILED";
            return;
        }

        auto myVtkObject = pThis->myVtkObject(myVtkData);

        if (!myVtkObject) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << " pThis->myVtkObject(myVtkData) FAILED";
            return;
        }

        auto myDownCastObject = MyVtk::SafeDownCast(myVtkObject);

        if (!myDownCastObject) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "MyVtk::SafeDownCast(myVtkObject) FAILED";
            return;
        }

        for(int index=0; index < list.count(); ++index)
        {
            auto& object = list.at(index);

            auto objUserData = dispatcher->lookup(object, renderData);

            if (!objUserData) {
                qWarning() << "YIKES!!" << Q_FUNC_INFO << "dispatcher->lookup(object, renderData) FAILED";
                continue;
            }

            auto objVtkObject = object->myVtkObject(objUserData);

            if (!objVtkObject) {
                qWarning() << "YIKES!!" << Q_FUNC_INFO << "object->myVtkObject(objUserData) FAILED";
                continue;
            }

            auto objDownCastObject = ObjVtk::SafeDownCast(objVtkObject);

            if (!objDownCastObject) {
                qWarning() << "YIKES!!" << Q_FUNC_INFO << "ObjVtk::SafeDownCast(objVtkObject) FAILED";
                return;
            }

            Rf(myDownCastObject, objDownCastObject, index);

            if (!dispatcher->unmap(object, ObjVtk::SafeDownCast(objVtkObject))) {
                qWarning().nospace() << "YIKES!! " << Q_FUNC_INFO << " dispatcher->unmap(object, renderData) FAILED";
                continue;
            }
        }
    }

    static void append(QQmlListProperty<ObjT>* l, ObjT* object)
    {
        if (object->m_quickVtkParent) {
            qWarning() << "YIKES!! object is attached to another object";
            return;
        }

        auto pThis = qobject_cast<MyT*>(l->object);

        if (!pThis) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "qobject_cast<MyT*>(l->object) FAILED";
            return;
        }

        auto* list = reinterpret_cast<QList<ObjT*>*>(l->data);

        if (!list) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "reinterpret_cast<QList<ObjT*>*>(l->data) FAILED";
            return;
        }

        if (list->contains(object)) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "appending an object already in the input list";
            return;
        }

        list->append(object);
        object->m_quickVtkParent = pThis;

        if (pThis->m_vtkInitialized)
        {
            auto dispatcher = pThis->m_weakDispatcher.lock();

            if (!dispatcher) {
                qWarning() << "YIKES!!" << Q_FUNC_INFO << pThis << "pThis->m_weakDispatcher.lock() FAILED";
                return;
            }

            dispatcher->dispatch_async([pThis = QPointer<MyT>(pThis), object = QPointer<ObjT>(object), index = list->count()]
                (vtkRenderWindow* renderWindow, typename ObjT::vtkUserData renderData) mutable
                {
                    if (!pThis) {
                        qWarning() << "YIKES!!" << Q_FUNC_INFO << "I was deleted";
                        return;
                    }

                    if (!object) {
                        qWarning() << "YIKES!!" << Q_FUNC_INFO << "object was deleted";
                        return;
                    }

                    attachObject(pThis, object, index, renderWindow, renderData);
                });
        }
    }

    static int count(QQmlListProperty<ObjT>* l)
    {
        auto pThis = qobject_cast<MyT*>(l->object);

        if (!pThis) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "qobject_cast<ObjT*>(l->object) FAILED";
            return -1;
        }

        auto* list = reinterpret_cast<QList<ObjT*>*>(l->data);

        if (!list) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "reinterpret_cast<QList<ObjT*>*>(l->data) FAILED";
            return -1;
        }

        return list->count();
    }

    static ObjT* at(QQmlListProperty<ObjT>* l, int i)
    {
        auto pThis = qobject_cast<MyT*>(l->object);

        if (!pThis) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "qobject_cast<ObjT*>(l->object) FAILED";
            return nullptr;
        }

        auto* list = reinterpret_cast<QList<ObjT*>*>(l->data);

        if (!list) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "reinterpret_cast<QList<ObjT*>*>(l->data) FAILED";
            return nullptr;
        }

        if (i<0 || i>=list->count()) {
            qWarning().nospace() << "YIKES!! " << Q_FUNC_INFO << " i(" << i << ") is out of bounds [" << 0 << "," << list->count() << "]";
            return nullptr;
        }

        return list->at(i);
    }

    static void clear(QQmlListProperty<ObjT>* l)
    {
        auto pThis = qobject_cast<MyT*>(l->object);

        if (!pThis) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "qobject_cast<ObjT*>(l->object) FAILED";
            return;
        }

        auto* list = reinterpret_cast<QList<ObjT*>*>(l->data);

        if (!list) {
            qWarning() << "YIKES!!" << Q_FUNC_INFO << "reinterpret_cast<QList<ObjT*>*>(l->data) FAILED";
            return;
        }

        for(auto object : *list)
        {
            if (object->m_quickVtkParent != pThis) {
                qWarning() << "YIKES!! object is not attached to me";
                continue;
            }
            object->m_quickVtkParent = nullptr;
        }


        if (pThis->m_vtkInitialized)
        {
            auto dispatcher = pThis->m_weakDispatcher.lock();

            if (!dispatcher) {
                qWarning() << "YIKES!!" << Q_FUNC_INFO << pThis << "pThis->m_weakDispatcher.lock() FAILED";
                return;
            }

            dispatcher->dispatch_async([pThis = QPointer<MyT>(pThis), list = *list] //<-- Note: we make a copy of the list
                (vtkRenderWindow* renderWindow, typename ObjT::vtkUserData renderData) mutable
                {
                    if (!pThis) {
                        qWarning() << "YIKES!!" << Q_FUNC_INFO << "I was deleted";
                        return;
                    }

                    detachObjects(pThis, renderData, list);
                });
        }

        list->clear();
    }
};