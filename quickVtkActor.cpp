#include "quickVtkActor.h"

#include "quickVtkDispatcher.h"
#include "quickVtkMapper.h"

#include <vtkActor.h>
#include <vtkMapper.h>
#include <vtkObject.h>

#include <QtCore/QPointer>
#include <QtCore/QDebug>

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<Actor>
{
    ~MyVtkData() { qDebug() << qobj; }

    vtkSmartPointer<vtkActor> actor;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

Actor::Actor(QObject* parent) : Prop3D(parent)
{
    qDebug() << this;
}

namespace {
void attachMapper(Actor* pThis, Mapper* mapper, vtkRenderWindow* renderWindow, Actor::vtkUserData renderData)
{
    if (!pThis) {
        qWarning() << "YIKES!! pThis is nullptr";
        return;
    }

    if (!mapper) {
        qWarning() << "YIKES!! mapper is nullptr";
        return;
    }

    auto dispatcher = pThis->m_weakDispatcher.lock();

    if (!dispatcher) {
        qWarning() << "YIKES!! m_weakDispatcher.lock() FAILED";
        return;
    }

    auto mapperData = dispatcher->lookup(mapper, renderData, true);

    if (!mapperData)
        mapperData = mapper->initializeVTK(pThis->m_weakDispatcher, renderWindow, renderData);

    if (!mapperData)
        return;

    auto vtkMapper = mapper->myVtkObject(mapperData);

    if (!vtkMapper)
        return;

    auto myUserData = dispatcher->lookup(pThis, renderData);

    if (!myUserData)
        return;

    auto vtkActor = pThis->myVtkObject(myUserData);

    if (!vtkActor)
        return;

    vtkActor->SetMapper(vtkMapper);
}

void detachMapper(Actor* pThis, Mapper* mapper, vtkRenderWindow* renderWindow, Actor::vtkUserData renderData)
{
    if (!pThis) {
        qWarning() << "YIKES!! pThis is nullptr";
        return;
    }

    if (!mapper) {
        qWarning() << "YIKES!! mapper is nullptr";
        return;
    }

    auto dispatcher = pThis->m_weakDispatcher.lock();

    if (!dispatcher) {
        qWarning() << "YIKES!! m_weakDispatcher.lock() FAILED";
        return;
    }

    auto mapperData = dispatcher->lookup(mapper, renderData, true);

    if (!mapperData)
        mapperData = mapper->initializeVTK(pThis->m_weakDispatcher, renderWindow, renderData);

    if (!mapperData)
        return;

    auto vtkMapper = mapper->myVtkObject(mapperData);

    if (!vtkMapper)
        return;

    auto myUserData = dispatcher->lookup(pThis, renderData);

    if (!myUserData)
        return;

    auto vtkActor = pThis->myVtkObject(myUserData);

    if (!vtkActor)
        return;

    if (vtkActor->GetMapper() != vtkMapper)
        qWarning() << "YIKES!! vtkActor->GetMapper():" << vtkActor->GetMapper() << "!= vtkMapper:" << vtkMapper;

    vtkActor->SetMapper(nullptr);
}
}

Actor::vtkUserData Actor::initializeVTK(WeakDispatcherPtr weakDispatcher, vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    qDebug() << this << m_vtkInitialized;

    auto vtk = vtkNew<MyVtkData>(this, weakDispatcher, renderData);

    vtk->actor = vtkActor::New();

    if (m_mapper)
    {
        attachMapper(this, m_mapper, renderWindow, renderData);
    }

    m_vtkInitialized = true;

    return vtk;
}

vtkActor* Actor::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->actor;
}

bool Actor::isVolatile() const
{
    return true;
}

Mapper* Actor::mapper() const { return m_mapper; }
void Actor::setMapper(Mapper* v)
{
    if (m_mapper == v)
        return;

    if (m_vtkInitialized && m_mapper)
    {
        if (v->m_quickVtkParent != this) {
            qWarning() << "YIKES!! mapper is not attached to me";
            return;
        }

        v->m_quickVtkParent = nullptr;

        auto dispatcher = m_weakDispatcher.lock();

        if (!dispatcher) {
            qWarning() << "YIKES!! m_weakDispatcher.lock() FAILED";
            return;
        }

        dispatcher->dispatch_async([pThis=QPointer<Actor>(this), mapper=QPointer<Mapper>(m_mapper)]
            (vtkRenderWindow* renderWindow, vtkUserData renderData) mutable
            {
                detachMapper(pThis, mapper, renderWindow, renderData);
            });
    }

    emit mapperChanged(m_mapper = v);

    if (m_vtkInitialized && m_mapper)
    {
        if (v->m_quickVtkParent) {
            qWarning() << "YIKES!! mapper is attached to another object";
            return;
        }

        v->m_quickVtkParent = this;

        auto dispatcher = m_weakDispatcher.lock();

        if (!dispatcher) {
            qWarning() << "YIKES!! m_weakDispatcher.lock() FAILED";
            return;
        }

        dispatcher->dispatch_async([pThis=QPointer<Actor>(this), mapper=QPointer<Mapper>(v)]
            (vtkRenderWindow* renderWindow, vtkUserData renderData) mutable
            {
                attachMapper(pThis, mapper, renderWindow, renderData);
            });
    }
}


} }