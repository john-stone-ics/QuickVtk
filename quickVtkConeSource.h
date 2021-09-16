#pragma once

#include "quickVtkPolyDataAlgorithm.h"

#include <vtkConeSource.h>

namespace quick { namespace vtk {

class ConeSource : public PolyDataAlgorithm
{
    Q_OBJECT
public:
    ConeSource(QObject* parent = nullptr);

    vtkConeSource* makeAlgorithm() override;

    vtkConeSource* myVtkObject(Object::vtkUserData) const override;
};

} }
