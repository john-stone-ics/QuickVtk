#include "quickVtkTexture.h"
#include "QProperty_setter_impl.h"

#include <QtCore/QDebug>

namespace quick { namespace vtk {

Texture::Texture(QObject* parent) : ImageAlgorithm(parent)
{}

Texture::Quality      Texture::quality     () const { return m_quality;      }
Texture::BlendingMode Texture::blendingMode() const { return m_blendingMode; }
bool                  Texture::repeat      () const { return m_repeat;       }
bool                  Texture::edgeClamp   () const { return m_edgeClamp;    }
bool                  Texture::interpolate () const { return m_interpolate;  }

void Texture::setBlendingMode(BlendingMode v, bool force) { QProperty_setter_impl(v, force, this, &Texture::m_blendingMode, &Texture::blendingModeChanged, &vtkTexture::SetBlendingMode); }
void Texture::setInterpolate (bool         v, bool force) { QProperty_setter_impl(v, force, this, &Texture::m_interpolate,  &Texture::interpolateChanged,  &vtkTexture::SetInterpolate);  }
void Texture::setEdgeClamp   (bool         v, bool force) { QProperty_setter_impl(v, force, this, &Texture::m_edgeClamp,    &Texture::edgeClampChanged,    &vtkTexture::SetEdgeClamp);    }
void Texture::setQuality     (Quality      v, bool force) { QProperty_setter_impl(v, force, this, &Texture::m_quality,      &Texture::qualityChanged,      &vtkTexture::SetQuality);      }
void Texture::setRepeat      (bool         v, bool force) { QProperty_setter_impl(v, force, this, &Texture::m_repeat,       &Texture::repeatChanged,       &vtkTexture::SetRepeat);       }

Texture::vtkUserData Texture::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto vtk = ImageAlgorithm::initializeVTK(renderWindow, renderData);

    setBlendingMode(m_blendingMode, true);
    setInterpolate(m_interpolate, true);
    setEdgeClamp(m_edgeClamp, true);
    setQuality(m_quality, true);
    setRepeat(m_repeat, true);

    return vtk;
}

vtkTexture *Texture::myVtkObject(vtkUserData userData) const
{
    auto vtkTexture = vtkTexture::SafeDownCast(ImageAlgorithm::myVtkObject(userData));

    if (!vtkTexture)
        qWarning() << "YIKES!! vtkTexture::SafeDownCast(ImageAlgorithm::myVtkObject(userData)) FAILED";

    return vtkTexture;
}

vtkTexture *Texture::makeAlgorithm()
{
    return vtkTexture::New();
}


} }
