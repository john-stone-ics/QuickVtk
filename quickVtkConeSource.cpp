#include "quickVtkConeSource.h"
#include "QProperty_setter_impl.h"

#include <vtkSmartPointer.h>

namespace quick { namespace vtk {

ConeSource::ConeSource(QObject* parent) : PolyDataAlgorithm(parent)
{
    m_onCenterChanged = [this]()
    {
        if (!m_vtkInitialized)
            return;

       dispatcher()->dispatch_async([
            coneSource = vtkSmartPointer<vtkConeSource>(myVtkObject()),
            center = m_center.m_values]
        (vtkRenderWindow*, vtkUserData)
        {
            coneSource->SetCenter(center.data());
        }, this);
    };
    connect(&m_center, &math::Vector3::xChanged, this, m_onCenterChanged);
    connect(&m_center, &math::Vector3::yChanged, this, m_onCenterChanged);
    connect(&m_center, &math::Vector3::zChanged, this, m_onCenterChanged);

    m_onDirectionChanged = [this]()
    {
        if (!m_vtkInitialized)
            return;

       dispatcher()->dispatch_async([
            coneSource = vtkSmartPointer<vtkConeSource>(myVtkObject()),
            direction = m_direction.m_values]
        (vtkRenderWindow*, vtkUserData)
        {
            coneSource->SetDirection(direction.data());
        }, this);
    };
    connect(&m_direction, &math::Vector3::xChanged, this, m_onDirectionChanged);
    connect(&m_direction, &math::Vector3::yChanged, this, m_onDirectionChanged);
    connect(&m_direction, &math::Vector3::zChanged, this, m_onDirectionChanged);
}

math::Vector3* ConeSource::center   () { return &m_center;    }
math::Vector3* ConeSource::direction() { return &m_direction; }

qreal ConeSource::resolution() const { return m_resolution; }
qreal ConeSource::capping   () const { return m_capping;    }
qreal ConeSource::radius    () const { return m_radius;     }
qreal ConeSource::height    () const { return m_height;     }
qreal ConeSource::angle     () const { return m_angle;      }

void ConeSource::setResolution(qreal v, bool force) { QProperty_setter_impl(v, force, this, &ConeSource::m_resolution, &ConeSource::resolutionChanged, &vtkConeSource::SetResolution); }
void ConeSource::setCapping   (qreal v, bool force) { QProperty_setter_impl(v, force, this, &ConeSource::m_capping,    &ConeSource::cappingChanged,    &vtkConeSource::SetCapping); }
void ConeSource::setRadius    (qreal v, bool force) { QProperty_setter_impl(v, force, this, &ConeSource::m_radius,     &ConeSource::radiusChanged,     &vtkConeSource::SetRadius);}
void ConeSource::setHeight    (qreal v, bool force) { QProperty_setter_impl(v, force, this, &ConeSource::m_height,     &ConeSource::heightChanged,     &vtkConeSource::SetHeight, static_cast<void(*)(vtkConeSource*,ConeSource*)>(
    [](vtkConeSource* vtkConeSource, ConeSource* pThis) {
        if (pThis->m_angle != vtkConeSource->GetAngle())
            emit pThis->angleChanged(pThis->m_angle = vtkConeSource->GetAngle());
    }));
}
void ConeSource::setAngle     (qreal v, bool force) { QProperty_setter_impl(v, force, this, &ConeSource::m_angle,      &ConeSource::angleChanged,      &vtkConeSource::SetAngle, static_cast<void(*)(vtkConeSource*,ConeSource*)>(
    [](vtkConeSource* vtkConeSource, ConeSource* pThis) {
        if (pThis->m_height != vtkConeSource->GetHeight())
            emit pThis->heightChanged(pThis->m_height = vtkConeSource->GetHeight());
    }));
}

ConeSource::vtkUserData ConeSource::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto myUserData = PolyDataAlgorithm::initializeVTK(renderWindow, renderData);

    m_onDirectionChanged();
    m_onCenterChanged();
    setResolution(m_resolution, true);
    setCapping(m_capping, true);
    setRadius(m_radius, true);
    setHeight(m_height, true);
    setAngle(m_angle, true);

    return myUserData;
}
vtkConeSource* ConeSource::myVtkObject(Object::vtkUserData myUserData) const
{
    auto vtkConeSource = vtkConeSource::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData));

    if (!vtkConeSource)
        qWarning() << "YIKES!! vtkConeSource::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData)) FAILED";

    return vtkConeSource;
}

vtkConeSource* ConeSource::makeAlgorithm()
{
    return vtkConeSource::New();
}


} }
