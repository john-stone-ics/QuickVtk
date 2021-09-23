#pragma once

#include <vtkObject.h>
#include <vtkSmartPointer.h>

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QVector>
#include <QtCore/QPointer>
#include <QtCore/QAtomicInteger>

class vtkRenderWindow;
class vtkObject;

namespace quick { namespace vtk {

class SharedData;

class Dispatcher
{
public:
    virtual ~Dispatcher() = default;

    // Note:  This class's methods will be called from both the gui-thread and render-thread (with gui-thread blocked)

    using vtkUserData = vtkSmartPointer<vtkObject>;
    virtual void dispatch_async(std::function<void(vtkRenderWindow*, vtkUserData)>, SharedData*) = 0;

    virtual bool map(QObject*, vtkUserData, vtkUserData) = 0;
    virtual bool unmap(QObject*, vtkUserData) = 0;
    virtual vtkUserData lookup(QObject*, vtkUserData, bool mightBeEmpty=false) = 0;

    void aboutToBeDeleted();
    struct Observer { virtual void aboutToBeDeleted() = 0; };
    QSet<Observer*> m_aboutToBeDeleted;
};

/*-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-= */

class SharedData
{
public:
    SharedData();
    virtual ~SharedData();

    void addVtkParent(QObject*);
    void delVtkParent(QObject*);

    Dispatcher* dispatcher();
    QVector<QPointer<QObject>> dispatchers();

    QAtomicInteger<bool> m_vtkInitialized = false;

private:
    std::list<QPointer<QObject>> m_quickVtkParents;
    bool findDispatchers(std::function<bool(Dispatcher*)>);
    void refreshCaches();
    Dispatcher*                m_dispatcher = nullptr;
    QVector<QPointer<QObject>> m_dispatchers;
    bool                       m_dirtyCache = true;
    static QSet<SharedData*>   s_instances;
};

/*-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-= */

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

