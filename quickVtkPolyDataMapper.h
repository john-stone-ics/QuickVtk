#pragma once

#include "quickVtkMapper.h"

#include <vtkPolyDataMapper.h>

namespace quick { namespace vtk {

class PolyDataMapper : public Mapper
{
    Q_OBJECT
public:
    PolyDataMapper(QObject* parent = nullptr);

    vtkPolyDataMapper* makeAlgorithm() override;

    vtkPolyDataMapper* myVtkObject(Object::vtkUserData) const override;
};

} }
