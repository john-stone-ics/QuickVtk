#pragma once

#include "quickVtkProp.h"

#include <vtkProp3D.h>

namespace quick { namespace vtk {

class Prop3D : public Prop
{
    Q_OBJECT
public:

//    Q_PROPERTY(quick::Math::Vector3* scale READ getScale CONSTANT);
//    Q_PROPERTY(quick::Math::Vector3* origin READ getOrigin CONSTANT);
//    Q_PROPERTY(quick::Math::Vector3* position READ getPosition CONSTANT);
//    Q_PROPERTY(quick::Math::Vector3* orientation READ getOrientation CONSTANT);

protected:
    Prop3D(QObject* parent);
};

} }
