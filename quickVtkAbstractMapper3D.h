#pragma once

#include "quickVtkAbstractMapper.h"

#include <vtkAbstractMapper3D.h>

namespace quick { namespace vtk {

class AbstractMapper3D : public AbstractMapper
{
    Q_OBJECT
protected:
    AbstractMapper3D(QObject* parent);
public:
    vtkAbstractMapper3D* myVtkObject(Object::vtkUserData) const override;
};

} }
