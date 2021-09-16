#include "VtkSource.h"

#include <vtkAlgorithm.h>

VtkSourceBase::VtkSourceBase(vtkAlgorithm* algorithm, QString name, QObject* parent) : QObject(parent), 
    algorithm(algorithm)
{
    setObjectName(name);
}

VtkSourceBase::~VtkSourceBase() = default;
