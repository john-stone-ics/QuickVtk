#include "quickVtkCylinderSource.h"

#include <vtkSmartPointer.h>

namespace quick { namespace vtk {

CylinderSource::CylinderSource(QObject* parent) : PolyDataAlgorithm(parent)
{
    qDebug() << this;

    m_onCenterChanged = [this](double)
    {
        if (!m_vtkInitialized)
            return;

        dispatcher()->dispatch_async([
            cylinderSource = vtkSmartPointer<vtkCylinderSource>(myVtkObject()),
            center = m_center.m_values]
        (vtkRenderWindow*, vtkUserData)
        {
            cylinderSource->SetCenter(center.data());
        }, this);
    };
    connect(&m_center, &math::Vector3::xChanged, this, m_onCenterChanged);
    connect(&m_center, &math::Vector3::yChanged, this, m_onCenterChanged);
    connect(&m_center, &math::Vector3::zChanged, this, m_onCenterChanged);
}

math::Vector3* CylinderSource::center   () { return &m_center;    }

qreal CylinderSource::height    () const { return m_height;     }
qreal CylinderSource::radius    () const { return m_radius;     }
qreal CylinderSource::resolution() const { return m_resolution; }
qreal CylinderSource::capping   () const { return m_capping;    }

void CylinderSource::setHeight(qreal v, bool force)
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
            vtkCylinderSource = vtkSmartPointer<vtkCylinderSource>(myVtkObject()),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkCylinderSource->SetHeight(v);
        }, this);
    }
}

void CylinderSource::setRadius(qreal v, bool force)
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
            vtkCylinderSource = vtkSmartPointer<vtkCylinderSource>(myVtkObject()),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkCylinderSource->SetRadius(v);
        }, this);
    }
}

void CylinderSource::setResolution(qreal v, bool force)
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
            vtkCylinderSource = vtkSmartPointer<vtkCylinderSource>(myVtkObject()),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkCylinderSource->SetResolution(v);
        }, this);
    }
}

void CylinderSource::setCapping(qreal v, bool force)
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
            vtkCylinderSource = vtkSmartPointer<vtkCylinderSource>(myVtkObject()),
            v = v]
        (vtkRenderWindow*, vtkUserData)
        {
            vtkCylinderSource->SetCapping(v);
        }, this);
    }
}

CylinderSource::vtkUserData CylinderSource::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto myUserData = PolyDataAlgorithm::initializeVTK(renderWindow, renderData);

    m_onCenterChanged(0);
    setHeight(m_height, true);
    setRadius(m_radius, true);
    setResolution(m_resolution, true);
    setCapping(m_capping, true);

    return myUserData;
}
vtkCylinderSource* CylinderSource::myVtkObject(Object::vtkUserData myUserData) const
{
    return vtkCylinderSource::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData));
}

vtkCylinderSource* CylinderSource::makeAlgorithm()
{
    return vtkCylinderSource::New();
}


} }
