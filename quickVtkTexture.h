#pragma once

#include "quickVtkImageAlgorithm.h"

#include <vtkTexture.h>

namespace quick { namespace vtk {

class Texture : public ImageAlgorithm {
    Q_OBJECT
public:
    Texture(QObject* paraent=nullptr);

    enum Quality {
        QualityDefault = VTK_TEXTURE_QUALITY_DEFAULT,
        Quality16Bit = VTK_TEXTURE_QUALITY_16BIT,
        Quality32Bit = VTK_TEXTURE_QUALITY_32BIT
    };
    Q_ENUM(Quality);
    Q_PROPERTY(quick::vtk::Texture::Quality quality READ quality WRITE setQuality NOTIFY qualityChanged);
    Quality quality() const;
    void setQuality(Quality, bool=false);
    Q_SIGNAL void qualityChanged(quick::vtk::Texture::Quality);
    Quality m_quality = QualityDefault;

    enum BlendingMode {
        BlendingNone =  vtkTexture::VTK_TEXTURE_BLENDING_MODE_NONE,
        BlendingReplace = vtkTexture::VTK_TEXTURE_BLENDING_MODE_REPLACE,
        BlendingModulate = vtkTexture::VTK_TEXTURE_BLENDING_MODE_MODULATE,
        BlendingAdd = vtkTexture::VTK_TEXTURE_BLENDING_MODE_ADD,
        BlendingAddSigned = vtkTexture::VTK_TEXTURE_BLENDING_MODE_ADD_SIGNED,
        BlendingInterpolate = vtkTexture::VTK_TEXTURE_BLENDING_MODE_INTERPOLATE,
        BlendingSubtract = vtkTexture::VTK_TEXTURE_BLENDING_MODE_SUBTRACT
    };
    Q_ENUM(BlendingMode);
    Q_PROPERTY(quick::vtk::Texture::BlendingMode blendingMode READ blendingMode WRITE setBlendingMode NOTIFY blendingModeChanged);
    BlendingMode blendingMode() const;
    void setBlendingMode(BlendingMode, bool=false);
    Q_SIGNAL void blendingModeChanged(quick::vtk::Texture::BlendingMode);
    BlendingMode m_blendingMode = BlendingNone;

    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat NOTIFY repeatChanged);
    bool repeat() const;
    void setRepeat(bool, bool=false);
    Q_SIGNAL void repeatChanged(bool);
    bool m_repeat = false;

    Q_PROPERTY(bool edgeClamp READ edgeClamp WRITE setEdgeClamp NOTIFY edgeClampChanged);
    bool edgeClamp() const;
    void setEdgeClamp(bool, bool=false);
    Q_SIGNAL void edgeClampChanged(bool);
    bool m_edgeClamp = false;

    Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate NOTIFY interpolateChanged);
    bool interpolate() const;
    void setInterpolate(bool, bool=false);
    Q_SIGNAL void interpolateChanged(bool);
    bool m_interpolate = false;

public:
    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkTexture* myVtkObject(vtkUserData) const override;

private:
    vtkTexture* makeAlgorithm() override;
};
} }
