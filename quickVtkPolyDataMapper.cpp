#include "quickVtkPolyDataMapper.h"

namespace quick { namespace vtk {

PolyDataMapper::PolyDataMapper(QObject* parent) : Mapper(parent)
{}

vtkPolyDataMapper* PolyDataMapper::makeAlgorithm() {
    return vtkPolyDataMapper::New();
}

vtkPolyDataMapper* PolyDataMapper::myVtkObject(Object::vtkUserData myUserData) const
{
    return vtkPolyDataMapper::SafeDownCast(Mapper::myVtkObject(myUserData));
}

} }
