#include "quickVtkAbstractMapper3D.h"

namespace quick { namespace vtk {

AbstractMapper3D::AbstractMapper3D(QObject* parent) : AbstractMapper(parent)
{}

vtkAbstractMapper3D* AbstractMapper3D::myVtkObject(vtkUserData myUserData) const
{
    return vtkAbstractMapper3D::SafeDownCast(AbstractMapper::myVtkObject(myUserData));
}

} }
