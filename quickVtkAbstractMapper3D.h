#pragma once

#include "quickVtkAbstractMapper.h"

namespace quick { namespace vtk {

class AbstractMapper3D : public AbstractMapper
{
    Q_OBJECT
protected:
    AbstractMapper3D(QObject* parent);
};

} }
