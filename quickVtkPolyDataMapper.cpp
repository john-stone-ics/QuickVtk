#include "quickVtkPolyDataMapper.h"

namespace quick { namespace vtk {

PolyDataMapper::PolyDataMapper(QObject* parent) : Mapper(parent)
{}


vtkPolyDataMapper* PolyDataMapper::myVtkObject(Object::vtkUserData myUserData) const
{
    auto vtkPolyDataMapper = vtkPolyDataMapper::SafeDownCast(Mapper::myVtkObject(myUserData));

    if (!vtkPolyDataMapper)
        qWarning() << "YIKES!! vtkPolyDataMapper::SafeDownCast(Mapper::myVtkObject(myUserData)) FAILED";

    return vtkPolyDataMapper;
}

vtkPolyDataMapper* PolyDataMapper::makeAlgorithm() {
    return vtkPolyDataMapper::New();
}

} }
