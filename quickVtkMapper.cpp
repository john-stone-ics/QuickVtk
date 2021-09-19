#include "quickVtkMapper.h"

#include <vtkMapper.h>
#include <vtkSmartPointer.h>

#include <QtCore/QDebug>

namespace quick { namespace vtk {

Mapper::Mapper(QObject* parent) : AbstractMapper3D(parent)
{}

Mapper::ScalarMode Mapper::scalarMode      () const { return m_scalarMode;       }
bool               Mapper::scalarVisibility() const { return m_scalarVisibility; }

void Mapper::setScalarMode(enum ScalarMode v, bool force)
{
    if (m_scalarMode != v || force)
    {
        if (m_scalarMode != v)
            emit scalarModeChanged(m_scalarMode = v);

        if (!m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

        dispatcher()->dispatch_async([
            pThis = QPointer<Mapper>(this),
            v = v]
        (vtkRenderWindow*, vtkUserData renderData)
        {
            if (!pThis) {
                qWarning() << "YIKES!! pThis was deleted";
                return;
            }

            auto dispatcher = pThis->dispatcher();

            if (!dispatcher) {
                qWarning() << "YIKES!! dispatcher was deleted";
                return;
            }

            auto vtkMapper = pThis->myVtkObject(dispatcher->lookup(pThis, renderData));

            if (!vtkMapper) {
                qWarning() << "YIKES!! vtkMapper is nullptr";
                return;
            }

            vtkMapper->SetScalarMode(int(v));
        });
    }
}

void Mapper::setScalarVisibility(bool v, bool force)
{
    if (m_scalarVisibility != v || force)
    {
        if (m_scalarVisibility != v)
            emit scalarVisibilityChanged(m_scalarVisibility = v);

        if (!m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

        dispatcher()->dispatch_async([
            pThis = QPointer<Mapper>(this),
            v = v]
        (vtkRenderWindow*, vtkUserData renderData)
        {
            if (!pThis) {
                qWarning() << "YIKES!! pThis was deleted";
                return;
            }

            auto dispatcher = pThis->dispatcher();

            if (!dispatcher) {
                qWarning() << "YIKES!! dispatcher was deleted";
                return;
            }

            auto vtkMapper = pThis->myVtkObject(dispatcher->lookup(pThis, renderData));

            if (!vtkMapper) {
                qWarning() << "YIKES!! vtkMapper is nullptr";
                return;
            }

            vtkMapper->SetScalarMode(int(v));
        });
    }
}

Mapper::vtkUserData Mapper::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData userData)
{
    auto myUserData = AbstractMapper3D::initializeVTK(renderWindow, userData);

    setScalarMode(m_scalarMode, true);
    setScalarVisibility(m_scalarVisibility, true);

    return myUserData;
}

vtkMapper* Mapper::myVtkObject(vtkUserData myUserData) const
{
    return vtkMapper::SafeDownCast(AbstractMapper3D::myVtkObject(myUserData));
}

} }

