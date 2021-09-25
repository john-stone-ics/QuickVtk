#pragma once

#include "quickVtkAlgorithm.h"

#include <vtkAbstractMapper.h>

namespace quick { namespace vtk {

class AbstractMapper : public Algorithm
{
    Q_OBJECT
protected:
    AbstractMapper(QObject* parent);

public:
    vtkUserData initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData) override;
    vtkAbstractMapper* myVtkObject(vtkUserData myUserData) const override;
    bool isVolatile() const override;
};

} }
