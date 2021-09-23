#include "quickVtkAbstractMapper3D.h"

namespace quick { namespace vtk {

AbstractMapper3D::AbstractMapper3D(QObject* parent) : AbstractMapper(parent)
{}

vtkAbstractMapper3D* AbstractMapper3D::myVtkObject(vtkUserData myUserData) const
{
    auto vtkAbstractMapper3D = vtkAbstractMapper3D::SafeDownCast(AbstractMapper::myVtkObject(myUserData));

    if (!vtkAbstractMapper3D)
        qWarning() << "vtkAbstractMapper3D::SafeDownCast(AbstractMapper::myVtkObject(myUserData)) FAILED";

    return vtkAbstractMapper3D;

}

} }
