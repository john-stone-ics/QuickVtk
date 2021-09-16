#pragma once

#include "quickVtkAlgorithm.h"

namespace quick { namespace vtk {

class AbstractMapper : public Algorithm
{
    Q_OBJECT
protected:
    AbstractMapper(QObject* parent);
};

} }
