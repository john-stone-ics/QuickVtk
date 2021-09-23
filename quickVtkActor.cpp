#include "quickVtkActor.h"

#include "quickVtkDispatcher.h"
#include "quickVtkMapper.h"

#include <vtkActor.h>
#include <vtkMapper.h>
#include <vtkObject.h>

#include <QtCore/QPointer>
#include <QtCore/QDebug>

namespace quick { namespace vtk {

Actor::Actor(QObject* parent) : Prop3D(parent)
{
    m_property.addVtkParent(this);
}

namespace {
void attachMapper(Actor* pThis, Mapper* mapper, vtkRenderWindow* renderWindow, Actor::vtkUserData renderData)
{
    if (!pThis) {
        qWarning() << "YIKES!! I was deleted";
        return;
    }

    if (!mapper) {
        qWarning() << "YIKES!! My mapper was deleted";
        return;
    }

    auto mapperData = pThis->dispatcher()->lookup(mapper, renderData, true);

    if (!mapperData)
        mapperData = mapper->initializeVTK(renderWindow, renderData);

    if (!mapperData)
        return;

    auto vtkMapper = mapper->myVtkObject(mapperData);

    if (!vtkMapper)
        return;

    auto vtkActor = pThis->myVtkObject(pThis->dispatcher()->lookup(pThis, renderData));

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

    auto mapperData = pThis->dispatcher()->lookup(mapper, renderData, true);

    if (!mapperData)
        mapperData = mapper->initializeVTK(renderWindow, renderData);

    if (!mapperData)
        return;

    auto vtkMapper = mapper->myVtkObject(mapperData);

    if (!vtkMapper)
        return;

    auto vtkActor = pThis->myVtkObject(pThis->dispatcher()->lookup(pThis, renderData));

    if (!vtkActor)
        return;

    if (vtkActor->GetMapper() != vtkMapper)
        qWarning() << "YIKES!! vtkActor->GetMapper():" << vtkActor->GetMapper() << "!= vtkMapper:" << vtkMapper;

    vtkActor->SetMapper(nullptr);
}
}

Mapper* Actor::mapper() const { return m_mapper; }
void Actor::setMapper(Mapper* v)
{
    if (m_mapper == v)
        return;

    if (m_mapper)
    {
        m_mapper->delVtkParent(this);

        if (m_vtkInitialized)
        {
            dispatcher()->dispatch_async([
                pThis = QPointer<Actor>(this),
                mapper = QPointer<Mapper>(m_mapper)]
            (vtkRenderWindow* renderWindow, vtkUserData renderData) mutable
            {
                detachMapper(pThis, mapper, renderWindow, renderData);
            }, this);
        }
    }

    emit mapperChanged(m_mapper = v);

    if (m_mapper)
    {
        m_mapper->addVtkParent(this);

        if (m_vtkInitialized)
        {
            dispatcher()->dispatch_async([
                pThis = QPointer<Actor>(this),
                mapper = QPointer<Mapper>(v)]
            (vtkRenderWindow* renderWindow, vtkUserData renderData) mutable
            {
                attachMapper(pThis, mapper, renderWindow, renderData);
            }, this);
        }
    }
}

Property* Actor::property() { return &m_property; }

Actor::vtkUserData Actor::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    m_property.initializeVTK(renderWindow, renderData);

    auto vtk = Prop3D::initializeVTK(renderWindow, renderData);

    if (m_mapper)
    {
        attachMapper(this, m_mapper, renderWindow, renderData);
    }

    return vtk;
}

vtkActor* Actor::myVtkObject(vtkUserData myUserData) const
{
    auto vtkActor = vtkActor::SafeDownCast(Prop3D::myVtkObject(myUserData));

    if (!vtkActor)
        qWarning() << "YIKES!! vtkActor::SafeDownCast(Prop3D::myVtkObject(myUserData)) FAILED";

    return vtkActor;
}

vtkActor* Actor::makeProp() const
{
    return vtkActor::New();
}


} }
