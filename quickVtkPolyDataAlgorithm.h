#pragma once

#include "quickVtkAlgorithm.h"

#include <vtkAlgorithm.h>

namespace quick { namespace vtk {

class PolyDataAlgorithm : public Algorithm
{
    Q_OBJECT
protected:
    PolyDataAlgorithm(QObject* parent);
};

} }
