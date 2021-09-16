#pragma once

#include "quickVtkAlgorithm.h"

namespace quick { namespace vtk {

class PolyDataAlgorithm : public Algorithm
{
    Q_OBJECT
protected:
    PolyDataAlgorithm(QObject* parent);
};

} }
