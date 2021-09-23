#include "quickVtkMapper.h"
#include "QProperty_setter_impl.h"

#include <vtkMapper.h>
#include <vtkSmartPointer.h>

#include <QtCore/QDebug>

namespace quick { namespace vtk {

Mapper::Mapper(QObject* parent) : AbstractMapper3D(parent)
{}

Mapper::ScalarMode Mapper::scalarMode      () const { return m_scalarMode;       }
bool               Mapper::scalarVisibility() const { return m_scalarVisibility; }

void Mapper::setScalarMode(ScalarMode v, bool force) { QProperty_setter_impl(v, force, this, &Mapper::m_scalarMode,       &Mapper::scalarModeChanged,       &vtkMapper::SetScalarMode);       }
void Mapper::setScalarVisibility(bool v, bool force) { QProperty_setter_impl(v, force, this, &Mapper::m_scalarVisibility, &Mapper::scalarVisibilityChanged, &vtkMapper::SetScalarVisibility); }

Mapper::vtkUserData Mapper::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData userData)
{
    auto myUserData = AbstractMapper3D::initializeVTK(renderWindow, userData);

    setScalarMode(m_scalarMode, true);
    setScalarVisibility(m_scalarVisibility, true);

    return myUserData;
}

vtkMapper* Mapper::myVtkObject(vtkUserData myUserData) const
{
    auto vtkMapper = vtkMapper::SafeDownCast(AbstractMapper3D::myVtkObject(myUserData));

    if (!vtkMapper)
        qWarning() << "YIKES!! vtkMapper::SafeDownCast(AbstractMapper3D::myVtkObject(myUserData)) FAILED";

    return vtkMapper;
}

} }

