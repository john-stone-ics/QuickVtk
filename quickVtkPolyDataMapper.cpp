#include "quickVtkPolyDataMapper.h"

namespace quick { namespace vtk {

PolyDataMapper::PolyDataMapper(QObject* parent) : Mapper(parent)
{
    qDebug() << this;
}

vtkPolyDataMapper* PolyDataMapper::makeAbstractMapper() {
    return vtkPolyDataMapper::New();
}

vtkPolyDataMapper* PolyDataMapper::myVtkObject(Object::vtkUserData myUserData) const
{
    return vtkPolyDataMapper::SafeDownCast(Mapper::myVtkObject(myUserData));
}

} }
