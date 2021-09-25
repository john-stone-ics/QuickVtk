#pragma once

#include "quickVtkPolyDataAlgorithm.h"
#include "quickMathVector3.h"

#include <vtkCylinderSource.h>

namespace quick { namespace vtk {

class CylinderSource : public PolyDataAlgorithm
{
    Q_OBJECT
public:
    CylinderSource(QObject* parent = nullptr);

    Q_PROPERTY(quick::math::Vector3* center READ center CONSTANT);
    math::Vector3* center();
    math::Vector3 m_center{0,0,0};
    std::function<void()> m_onCenterChanged;

    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged);
    qreal height() const;
    void setHeight(qreal, bool force=false);
    Q_SIGNAL void heightChanged(qreal);
    qreal m_height = 1.0;

    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged);
    qreal radius() const;
    void setRadius(qreal, bool force=false);
    Q_SIGNAL void radiusChanged(qreal);
    qreal m_radius = 0.5;

    Q_PROPERTY(int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
    qreal resolution() const;
    void setResolution(qreal, bool force=false);
    Q_SIGNAL void resolutionChanged(qreal);
    qreal m_resolution = 6;

    Q_PROPERTY(bool capping READ capping WRITE setCapping NOTIFY cappingChanged);
    qreal capping() const;
    void setCapping(qreal, bool force=false);
    Q_SIGNAL void cappingChanged(qreal);
    qreal m_capping = 1;

    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkCylinderSource* myVtkObject(Object::vtkUserData) const override;
    vtkCylinderSource* makeAlgorithm() override;
};

} }
