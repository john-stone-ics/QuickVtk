#pragma once

#include <vtkObject.h>
#include <vtkSmartPointer.h>

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QThread>
#include <QtCore/QAtomicInteger>
#include <QtCore/QAtomicPointer>
#include <QtCore/QRecursiveMutex>
#include <QtCore/QExplicitlySharedDataPointer>

#include <functional>

class vtkRenderWindow;
class vtkObject;

namespace quick { namespace vtk {

struct WeakDispatcherPtr;

class Object;

class Dispatcher
{
public:
    virtual ~Dispatcher() = default;

    // Note:  This class's methods will be called from both the gui-thread and render-thread (with gui-thread blocked)

    using vtkUserData = vtkSmartPointer<vtkObject>;
    virtual void dispatch_async(std::function<void(vtkRenderWindow*, vtkUserData)>&&) = 0;

    virtual bool map(QObject*, vtkUserData, vtkUserData) = 0;
    virtual bool unmap(QObject*, vtkUserData) = 0;
    virtual vtkUserData lookup(QObject*, vtkUserData, bool mightBeEmpty=false) = 0;
};


struct StrongDispatcherPtr
{
    StrongDispatcherPtr(WeakDispatcherPtr&);

    quick::vtk::Dispatcher* operator->();
    operator quick::vtk::Dispatcher*();

    struct Data : QSharedData {
        Data(WeakDispatcherPtr&);
        ~Data();
        WeakDispatcherPtr& _wp;
    };
    QExplicitlySharedDataPointer<Data> d;
};

struct WeakDispatcherPtr
{
    WeakDispatcherPtr(quick::vtk::Dispatcher *);

    StrongDispatcherPtr lock();
    operator bool();

    struct Data : QSharedData {
        Data(quick::vtk::Dispatcher*);
        ~Data();
        QRecursiveMutex         _m;
        int                     _n = 0;
        Qt::HANDLE              _t = nullptr;
        quick::vtk::Dispatcher* _p = nullptr;
        QMetaObject::Connection _c;
    };
    QExplicitlySharedDataPointer<Data> d;
};


struct SharedData
{
    WeakDispatcherPtr       m_weakDispatcher{nullptr};
    QAtomicInteger<bool>    m_vtkInitialized = false;
    QPointer<QObject>       m_quickVtkParent;
};

template<typename T>
struct UserData : vtkObject {
    virtual ~UserData() {
        if (qobj)
            qobj->m_vtkInitialized = false;
    }
    QPointer<T> qobj;
};

template<typename T, typename Q>
::vtkNew<T> vtkNew(Q* qobj, WeakDispatcherPtr weakDispatcher, Dispatcher::vtkUserData renderData = {})
{
    qobj->m_weakDispatcher = weakDispatcher;

    ::vtkNew<T> tmp;

    tmp->qobj = qobj;

    auto dispatcher = weakDispatcher.lock();

    if (!dispatcher)
        qFatal("YIKES!! %s m_weakDispatcher.lock() FAILED", Q_FUNC_INFO);

    if (!dispatcher->map(qobj, tmp, renderData))
        qFatal("YIKES!! %s dispatcher->map(obj, myData, renderData) FAILED", Q_FUNC_INFO);

    return tmp;
}

}}

