#pragma once

#include <vtkObject.h>
#include <vtkSmartPointer.h>

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#if 0
#include <QtCore/QThread>
#include <QtCore/QAtomicInteger>
#include <QtCore/QAtomicPointer>
#include <QtCore/QRecursiveMutex>
#include <QtCore/QExplicitlySharedDataPointer>
#endif

#include <list>
#include <functional>

class vtkRenderWindow;
class vtkObject;

namespace quick { namespace vtk {

struct WeakDispatcherPtr;

class Object;
class SharedData;

class Dispatcher
{
public:
    virtual ~Dispatcher() = default;

    // Note:  This class's methods will be called from both the gui-thread and render-thread (with gui-thread blocked)

    using vtkUserData = vtkSmartPointer<vtkObject>;
    virtual void dispatch_async(std::function<void(vtkRenderWindow*, vtkUserData)>&&, SharedData*) = 0;

    virtual bool map(QObject*, vtkUserData, vtkUserData) = 0;
    virtual bool unmap(QObject*, vtkUserData) = 0;
    virtual vtkUserData lookup(QObject*, vtkUserData, bool mightBeEmpty=false) = 0;

    void aboutToBeDeleted();
    struct Observer { virtual void aboutToBeDeleted() = 0; };
    QSet<Observer*> m_aboutToBeDeleted;
};

#if 0
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

    StrongDispatcherPtr lock(bool canFail=false);
    operator bool() const;

    struct Data : QSharedData, Dispatcher::Observer {
        Data(quick::vtk::Dispatcher*);
        ~Data();
        QRecursiveMutex         _m;
        int                     _n = 0;
        Qt::HANDLE              _t = nullptr;
        quick::vtk::Dispatcher* _p = nullptr;
        void aboutToBeDeleted() override;
    };
    QExplicitlySharedDataPointer<Data> d;
};
#endif

class SharedData
{
public:
    SharedData();
    virtual ~SharedData();

    void addVtkParent(QObject*);
    void delVtkParent(QObject*);

    Dispatcher* dispatcher();
    QList<QPointer<QObject>> dispatchers();

    QAtomicInteger<bool> m_vtkInitialized = false;

private:
    std::list<QPointer<QObject>> m_quickVtkParents;
    bool findDispatchers(std::function<bool(Dispatcher*)>);
    void refreshCaches();
    Dispatcher*              m_dispatcher = nullptr;
    QList<QPointer<QObject>> m_dispatchers;
    bool                     m_dirtyCache = true;
    static QSet<SharedData*> s_instances;
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
::vtkNew<T> vtkNew(Q* qobj, Dispatcher::vtkUserData renderData = {})
{
    ::vtkNew<T> tmp;

    tmp->qobj = qobj;

    if (!qobj->dispatcher()->map(qobj, tmp, renderData))
        qFatal("YIKES!! %s dispatcher->map(obj, myData, renderData) FAILED", Q_FUNC_INFO);

    return tmp;
}

}}

