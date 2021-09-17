#include "quickVtkMapper.h"

#include <vtkMapper.h>
#include <vtkSmartPointer.h>

#include <QtCore/QDebug>

namespace quick { namespace vtk {

Mapper::Mapper(QObject* parent) : AbstractMapper3D(parent)
{}

vtkMapper* Mapper::myVtkObject(vtkUserData myUserData) const
{
    return vtkMapper::SafeDownCast(AbstractMapper3D::myVtkObject(myUserData));
}

Mapper::ScalarMode Mapper::scalarMode() const
{
    return m_scalarMode;
}
void Mapper::setScalarMode(enum ScalarMode v)
{
    if (m_scalarMode != v)
        emit scalarModeChanged(m_scalarMode = v);
}

bool Mapper::scalarVisibility() const
{
    return m_scalarVisibility;
}
void Mapper::setScalarVisibility(bool v)
{
    if (m_scalarVisibility != v)
        emit scalarVisibilityChanged(m_scalarVisibility = v);
}

} }
