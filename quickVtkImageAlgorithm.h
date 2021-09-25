#pragma once

#include "quickVtkAlgorithm.h"

#include <vtkImageAlgorithm.h>

namespace quick { namespace vtk {

class ImageAlgorithm : public Algorithm {
    Q_OBJECT
protected:
    ImageAlgorithm(QObject* parent);
public:
    vtkUserData initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData) override;
    vtkImageAlgorithm* myVtkObject(vtkUserData) const override;
    bool isVolatile() const override;
};

} }
