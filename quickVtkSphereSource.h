#pragma once

#include "quickVtkPolyDataAlgorithm.h"
#include "quickMathVector3.h"

#include <vtkSphereSource.h>

namespace quick { namespace vtk {

class SphereSource : public PolyDataAlgorithm
{
    Q_OBJECT
public:
    SphereSource(QObject* parent = nullptr);

    Q_PROPERTY(quick::math::Vector3* center READ center CONSTANT);
    math::Vector3* center();
    math::Vector3 m_center{0,0,0};
    std::function<void()> m_onCenterChanged;

    Q_PROPERTY(bool latLongTessellation READ latLongTessellation WRITE setLatLongTessellation NOTIFY latLongTessellationChanged);
    bool latLongTessellation() const;
    void setLatLongTessellation(bool, bool=false);
    Q_SIGNAL void latLongTessellationChanged(bool);
    bool m_latLongTessellation = false;

    Q_PROPERTY(int thetaResolution READ thetaResolution WRITE setThetaResolution NOTIFY thetaResolutionChanged);
    int thetaResolution() const;
    void setThetaResolution(int, bool=false);
    Q_SIGNAL void thetaResolutionChanged(int);
    int m_thetaResolution = 6;

    Q_PROPERTY(int phiResolution READ phiResolution WRITE setPhiResolution NOTIFY phiResolutionChanged);
    int phiResolution() const;
    void setPhiResolution(int, bool=false);
    Q_SIGNAL void phiResolutionChanged(int);
    int m_phiResolution = 6;

    Q_PROPERTY(qreal startTheta READ startTheta WRITE setStartTheta NOTIFY startThetaChanged);
    qreal startTheta() const;
    void setStartTheta(qreal, bool=false);
    Q_SIGNAL void startThetaChanged(qreal);
    qreal m_startTheta = 0.0;

    Q_PROPERTY(qreal startPhi READ startPhi WRITE setStartPhi NOTIFY startPhiChanged);
    qreal startPhi() const;
    void setStartPhi(qreal, bool=false);
    Q_SIGNAL void startPhiChanged(qreal);
    qreal m_startPhi = 0.0;

    Q_PROPERTY(qreal endTheta READ endTheta WRITE setEndTheta NOTIFY endThetaChanged);
    qreal endTheta() const;
    void setEndTheta(qreal, bool=false);
    Q_SIGNAL void endThetaChanged(qreal);
    qreal m_endTheta = 360.0;

    Q_PROPERTY(qreal endPhi READ endPhi WRITE setEndPhi NOTIFY endPhiChanged);
    qreal endPhi() const;
    void setEndPhi(qreal, bool=false);
    Q_SIGNAL void endPhiChanged(qreal);
    qreal m_endPhi = 180.0;

    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged);
    qreal radius() const;
    void setRadius(qreal, bool=false);
    Q_SIGNAL void radiusChanged(qreal);
    qreal m_radius = 0.5;

public:
    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkSphereSource* myVtkObject(Object::vtkUserData) const override;
private:
    vtkSphereSource* makeAlgorithm() override;
};

} }
