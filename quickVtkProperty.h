#pragma once

#include "quickVtkObject.h"

#include <vtkProperty.h>

#include <QtGui/QColor>

namespace quick { namespace vtk {

class Property : public Object {
    Q_OBJECT
public:    
    Property(QObject* parent=nullptr);

    enum Interpolation {
        Flat = VTK_FLAT,
        Gouraud = VTK_GOURAUD,
        Phong = VTK_PHONG
    };
    Q_ENUM(Interpolation);
    Q_PROPERTY(quick::vtk::Property::Interpolation interpolation READ interpolation WRITE setInterpolation NOTIFY interpolationChanged)
    Interpolation interpolation() const;
    void setInterpolation(Interpolation, bool=false);
    Q_SIGNAL void interpolationChanged(Interpolation);
    Interpolation m_interpolation = Gouraud;

    enum Representation {
        Points = VTK_POINTS,
        Wireframe = VTK_WIREFRAME,
        Surface = VTK_SURFACE
    };
    Q_ENUM(Representation);
    Q_PROPERTY(quick::vtk::Property::Representation representation READ representation WRITE setRepresentation NOTIFY representationChanged)
    Representation representation() const;
    void setRepresentation(Representation, bool=false);
    Q_SIGNAL void representationChanged(Representation);
    Representation m_representation = Surface;

    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    qreal opacity() const;
    void setOpacity(qreal, bool=false);
    Q_SIGNAL void opacityChanged(qreal);
    qreal m_opacity = 1.0;

    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    qreal lineWidth() const;
    void setLineWidth(qreal, bool=false);
    Q_SIGNAL void lineWidthChanged(qreal);
    qreal m_lineWidth = 1.0;

    Q_PROPERTY(qreal pointSize READ pointSize WRITE setPointSize NOTIFY pointSizeChanged)
    qreal pointSize() const;
    void setPointSize(qreal, bool=false);
    Q_SIGNAL void pointSizeChanged(qreal);
    qreal m_pointSize = 1.0;

    Q_PROPERTY(bool shading READ shading WRITE setShading NOTIFY shadingChanged)
    bool shading() const;
    void setShading(bool, bool=false);
    Q_SIGNAL void shadingChanged(bool);
    bool m_shading = false;

    Q_PROPERTY(bool lighting READ lighting WRITE setLighting NOTIFY lightingChanged)
    bool lighting() const;
    void setLighting(bool, bool=false);
    Q_SIGNAL void lightingChanged(bool);
    bool m_lighting = true;

    Q_PROPERTY(bool edgeVisibility READ edgeVisibility WRITE setEdgeVisibility NOTIFY edgeVisibilityChanged)
    bool edgeVisibility() const;
    void setEdgeVisibility(bool, bool=false);
    Q_SIGNAL void edgeVisibilityChanged(bool);
    bool m_edgeVisibility = false;

    Q_PROPERTY(bool backfaceCulling READ backfaceCulling WRITE setBackfaceCulling NOTIFY backfaceCullingChanged)
    bool backfaceCulling() const;
    void setBackfaceCulling(bool, bool=false);
    Q_SIGNAL void backfaceCullingChanged(bool);
    bool m_backfaceCulling = false;

    Q_PROPERTY(bool frontfaceCulling READ frontfaceCulling WRITE setFrontfaceCulling NOTIFY frontfaceCullingChanged)
    bool frontfaceCulling() const;
    void setFrontfaceCulling(bool, bool=false);
    Q_SIGNAL void frontfaceCullingChanged(bool);
    bool m_frontfaceCulling = false;

    Q_PROPERTY(QColor specularColor READ specularColor WRITE setSpecularColor NOTIFY specularColorChanged);
    QColor specularColor() const;
    void setSpecularColor(QColor, bool=false);
    Q_SIGNAL void specularColorChanged(QColor);
    QColor m_specularColor{255,255,255};

    Q_PROPERTY(QColor diffuseColor READ diffuseColor WRITE setDiffuseColor NOTIFY diffuseColorChanged);
    QColor diffuseColor() const;
    void setDiffuseColor(QColor, bool=false);
    Q_SIGNAL void diffuseColorChanged(QColor);
    QColor m_diffuseColor{255,255,255};

    Q_PROPERTY(QColor ambientColor READ ambientColor WRITE setAmbientColor NOTIFY ambientColorChanged);
    QColor ambientColor() const;
    void setAmbientColor(QColor, bool=false);
    Q_SIGNAL void ambientColorChanged(QColor);
    QColor m_ambientColor{255,255,255};

    Q_PROPERTY(QColor edgeColor READ edgeColor WRITE setEdgeColor NOTIFY edgeColorChanged);
    QColor edgeColor() const;
    void setEdgeColor(QColor, bool=false);
    Q_SIGNAL void edgeColorChanged(QColor);
    QColor m_edgeColor{0,0,0};

    Q_PROPERTY(qreal diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseChanged);
    qreal diffuse() const;
    void setDiffuse(qreal, bool=false);
    Q_SIGNAL void diffuseChanged(qreal);
    qreal m_diffuse = 1.0;

    Q_PROPERTY(qreal ambient READ ambient WRITE setAmbient NOTIFY ambientChanged);
    qreal ambient() const;
    void setAmbient(qreal, bool=false);
    Q_SIGNAL void ambientChanged(qreal);
    qreal m_ambient = 0.0;

    Q_PROPERTY(qreal specular READ specular WRITE setSpecular NOTIFY specularChanged);
    qreal specular() const;
    void setSpecular(qreal, bool=false);
    Q_SIGNAL void specularChanged(qreal);
    qreal m_specular = 0.0;

    Q_PROPERTY(qreal specularPower READ specularPower WRITE setSpecularPower NOTIFY specularPowerChanged);
    qreal specularPower() const;
    void setSpecularPower(qreal, bool=false);
    Q_SIGNAL void specularPowerChanged(qreal);
    qreal m_specularPower = 1.0;

public:
    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkProperty* myVtkObject(vtkUserData) const override;
    bool isVolatile() const override;

};
} }
