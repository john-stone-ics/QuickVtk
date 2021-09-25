#include "quickVtkSphereSource.h"
#include "QProperty_setter_impl.h"

#include <vtkSmartPointer.h>

namespace quick { namespace vtk {

SphereSource::SphereSource(QObject* parent) : PolyDataAlgorithm(parent)
{
    m_onCenterChanged = [this]()
    {
        if (!m_vtkInitialized)
            return;

       dispatcher()->dispatch_async([
            sphereSource = vtkSmartPointer<vtkSphereSource>(myVtkObject(nullptr)),
            center = m_center.m_values]
        (vtkRenderWindow*, vtkUserData)
        {
            sphereSource->SetCenter(center.data());
        }, this);
    };
    connect(&m_center, &math::Vector3::xChanged, this, m_onCenterChanged);
    connect(&m_center, &math::Vector3::yChanged, this, m_onCenterChanged);
    connect(&m_center, &math::Vector3::zChanged, this, m_onCenterChanged);
}

math::Vector3* SphereSource::center   () { return &m_center;    }

bool  SphereSource::latLongTessellation() const { return m_latLongTessellation; }
int   SphereSource::thetaResolution    () const { return m_thetaResolution;     }
int   SphereSource::phiResolution      () const { return m_phiResolution;       }
qreal SphereSource::startTheta         () const { return m_startTheta;          }
qreal SphereSource::startPhi           () const { return m_startPhi;            }
qreal SphereSource::endTheta           () const { return m_endTheta;            }
qreal SphereSource::endPhi             () const { return m_endPhi;              }
qreal SphereSource::radius             () const { return m_radius;              }

void SphereSource::setLatLongTessellation(bool  v, bool force) { QProperty_setter_impl(v, force, this, &SphereSource::m_latLongTessellation, &SphereSource::latLongTessellationChanged, &vtkSphereSource::SetLatLongTessellation);}
void SphereSource::setThetaResolution    (int   v, bool force) { QProperty_setter_impl(v, force, this, &SphereSource::m_thetaResolution,     &SphereSource::thetaResolutionChanged,     &vtkSphereSource::SetThetaResolution    );}
void SphereSource::setPhiResolution      (int   v, bool force) { QProperty_setter_impl(v, force, this, &SphereSource::m_phiResolution,       &SphereSource::phiResolutionChanged,       &vtkSphereSource::SetPhiResolution      );}
void SphereSource::setStartTheta         (qreal v, bool force) { QProperty_setter_impl(v, force, this, &SphereSource::m_startTheta,          &SphereSource::startThetaChanged,          &vtkSphereSource::SetStartTheta         );}
void SphereSource::setStartPhi           (qreal v, bool force) { QProperty_setter_impl(v, force, this, &SphereSource::m_startPhi,            &SphereSource::startPhiChanged,            &vtkSphereSource::SetStartPhi           );}
void SphereSource::setEndTheta           (qreal v, bool force) { QProperty_setter_impl(v, force, this, &SphereSource::m_endTheta,            &SphereSource::endThetaChanged,            &vtkSphereSource::SetEndTheta           );}
void SphereSource::setEndPhi             (qreal v, bool force) { QProperty_setter_impl(v, force, this, &SphereSource::m_endPhi,              &SphereSource::endPhiChanged,              &vtkSphereSource::SetEndPhi             );}
void SphereSource::setRadius             (qreal v, bool force) { QProperty_setter_impl(v, force, this, &SphereSource::m_radius,              &SphereSource::radiusChanged,              &vtkSphereSource::SetRadius             );}

SphereSource::vtkUserData SphereSource::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto myUserData = PolyDataAlgorithm::initializeVTK(renderWindow, renderData);

    m_onCenterChanged();
    setLatLongTessellation(m_latLongTessellation, true);
    setThetaResolution(m_thetaResolution, true);
    setPhiResolution(m_phiResolution, true);
    setStartTheta(m_startTheta, true);
    setStartPhi(m_startPhi, true);
    setEndTheta(m_endTheta, true);
    setEndPhi(m_endPhi, true);
    setRadius(m_radius, true);

    return myUserData;
}

vtkSphereSource* SphereSource::myVtkObject(Object::vtkUserData myUserData) const
{
    auto vtkSphereSource = vtkSphereSource::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData));

    if (!vtkSphereSource)
        qWarning() << "YIKES!! vtkSphereSource::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData)) FAILED";

    return vtkSphereSource;
}

vtkSphereSource* SphereSource::makeAlgorithm()
{
    return vtkSphereSource::New();
}


} }
