#include "quickVtkConeSource.h"

namespace quick { namespace vtk {

ConeSource::ConeSource(QObject* parent) : PolyDataAlgorithm(parent)
{}

vtkConeSource* ConeSource::makeAlgorithm() {
    return vtkConeSource::New();
}

vtkConeSource* ConeSource::myVtkObject(Object::vtkUserData myUserData) const
{
    return vtkConeSource::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData));
}

} }
