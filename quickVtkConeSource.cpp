#include "quickVtkConeSource.h"

#include <vtkSmartPointer.h>

namespace quick { namespace vtk {

ConeSource::ConeSource(QObject* parent) : PolyDataAlgorithm(parent)
{
    qDebug() << this;

    m_onCenterChanged = [this](double)
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

    m_onDirectionChanged = [this](double)
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

qreal ConeSource::height    () const { return m_height;     }
qreal ConeSource::radius    () const { return m_radius;     }
qreal ConeSource::angle     () const { return m_angle;      }
qreal ConeSource::resolution() const { return m_resolution; }
qreal ConeSource::capping   () const { return m_capping;    }

void ConeSource::setHeight(qreal v, bool force)
{
    if (m_height != v || force)
    {
        if (m_height != v)
            emit heightChanged(m_height = v);

        if (!m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

       dispatcher()->dispatch_async([
            vtkConeSource = vtkSmartPointer<vtkConeSource>(myVtkObject()),
            pThis = QPointer<ConeSource>(this),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkConeSource->SetHeight(v);

            if (vtkConeSource->GetAngle() != pThis->m_angle)
                emit pThis->angleChanged(pThis->m_angle = vtkConeSource->GetAngle());
        }, this);
    }
}

void ConeSource::setRadius(qreal v, bool force)
{
    if (m_radius != v || force)
    {
        if (m_radius != v)
            emit radiusChanged(m_radius = v);

        if (!m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

       dispatcher()->dispatch_async([
            vtkConeSource = vtkSmartPointer<vtkConeSource>(myVtkObject()),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkConeSource->SetRadius(v);
        }, this);
    }
}

void ConeSource::setAngle(qreal v, bool force)
{
    if (m_angle != v || force)
    {
        if (m_angle != v)
            emit angleChanged(m_angle = v);

        if (!m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

       dispatcher()->dispatch_async([
            vtkConeSource = vtkSmartPointer<vtkConeSource>(myVtkObject()),
            pThis = QPointer<ConeSource>(this),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkConeSource->SetAngle(v);

            if (vtkConeSource->GetHeight() != pThis->m_height)
                emit pThis->heightChanged(pThis->m_height = vtkConeSource->GetHeight());
        }, this);
    }
}

void ConeSource::setResolution(qreal v, bool force)
{
    if (m_resolution != v || force)
    {
        if (m_resolution != v)
            emit resolutionChanged(m_resolution = v);

        if (!m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

       dispatcher()->dispatch_async([
            vtkConeSource = vtkSmartPointer<vtkConeSource>(myVtkObject()),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkConeSource->SetResolution(v);
        }, this);
    }
}

void ConeSource::setCapping(qreal v, bool force)
{
    if (m_capping != v || force)
    {
        if (m_capping != v)
            emit cappingChanged(m_capping = v);

        if (!m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

       dispatcher()->dispatch_async([
            vtkConeSource = vtkSmartPointer<vtkConeSource>(myVtkObject()),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkConeSource->SetCapping(v);
        }, this);
    }
}

ConeSource::vtkUserData ConeSource::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto myUserData = PolyDataAlgorithm::initializeVTK(renderWindow, renderData);

    m_onDirectionChanged(0);
    m_onCenterChanged(0);
    setHeight(m_height, true);
    setRadius(m_radius, true);
    setAngle(m_angle, true);
    setResolution(m_resolution, true);
    setCapping(m_capping, true);

    return myUserData;
}
vtkConeSource* ConeSource::myVtkObject(Object::vtkUserData myUserData) const
{
    return vtkConeSource::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData));
}

vtkConeSource* ConeSource::makeAlgorithm()
{
    return vtkConeSource::New();
}


} }
