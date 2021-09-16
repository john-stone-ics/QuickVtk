#pragma once

#include "quickVtkImplicitFunction.h"

#include <vtkCone.h>

namespace quick { namespace vtk {

class Cone : public ImplicitFunction
{
    Q_OBJECT
public:
    Cone(QObject* parent = nullptr);

    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    qreal angle() const;
    void setAngle(qreal);
    Q_SIGNAL void angleChanged(qreal);
    qreal m_angle;

public:
    vtkUserData initializeVTK(WeakDispatcherPtr, vtkRenderWindow*, vtkUserData) override;
    vtkCone* myVtkObject(vtkUserData) const override;
};

} }
