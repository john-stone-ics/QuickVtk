#include "quickVtkProperty.h"

#include "QProperty_setter_impl.h"

namespace quick { namespace vtk {


namespace {
struct MyVtkData : UserData<Property>
{
    vtkSmartPointer<vtkProperty> property;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

Property::Property(QObject *parent) : Object(Object::Type::Other, parent)
{}

Property::Interpolation  Property::interpolation   () const { return m_interpolation;    }
Property::Representation Property::representation  () const { return m_representation;   }
qreal                    Property::opacity         () const { return m_opacity;          }
qreal                    Property::lineWidth       () const { return m_lineWidth;        }
qreal                    Property::pointSize       () const { return m_pointSize;        }
bool                     Property::shading         () const { return m_shading;          }
bool                     Property::lighting        () const { return m_lighting;         }
bool                     Property::edgeVisibility  () const { return m_edgeVisibility;   }
bool                     Property::backfaceCulling () const { return m_backfaceCulling;  }
bool                     Property::frontfaceCulling() const { return m_frontfaceCulling; }
QColor                   Property::specularColor   () const { return m_specularColor;    }
QColor                   Property::diffuseColor    () const { return m_diffuseColor;     }
QColor                   Property::ambientColor    () const { return m_ambientColor;     }
QColor                   Property::edgeColor       () const { return m_edgeColor;        }
qreal                    Property::ambient         () const { return m_ambient;          }
qreal                    Property::diffuse         () const { return m_diffuse;          }
qreal                    Property::specular        () const { return m_specular;         }
qreal                    Property::specularPower   () const { return m_specularPower;    }

namespace {
struct vtkColor { double c[3]; operator double*() { return c; } };
vtkColor qcolor2vtk(QColor c) { return { c.redF(), c.greenF(), c.blueF()}; }
}
void Property::setInterpolation   (Property::Interpolation  v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_interpolation,    &Property::interpolationChanged,    &vtkProperty::SetInterpolation );             }
void Property::setRepresentation  (Property::Representation v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_representation,   &Property::representationChanged,   &vtkProperty::SetRepresentation);             }
void Property::setOpacity         (qreal                    v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_opacity,          &Property::opacityChanged,          &vtkProperty::SetOpacity);                    }
void Property::setLineWidth       (qreal                    v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_lineWidth,        &Property::lineWidthChanged,        &vtkProperty::SetLineWidth);                  }
void Property::setPointSize       (qreal                    v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_pointSize,        &Property::pointSizeChanged,        &vtkProperty::SetPointSize);                  }
void Property::setShading         (bool                     v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_shading,          &Property::shadingChanged,          &vtkProperty::SetShading);                    }
void Property::setLighting        (bool                     v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_lighting,         &Property::lightingChanged,         &vtkProperty::SetLighting);                   }
void Property::setEdgeVisibility  (bool                     v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_edgeVisibility,   &Property::edgeVisibilityChanged,   &vtkProperty::SetEdgeVisibility);             }
void Property::setBackfaceCulling (bool                     v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_backfaceCulling,  &Property::backfaceCullingChanged,  &vtkProperty::SetBackfaceCulling);            }
void Property::setFrontfaceCulling(bool                     v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_frontfaceCulling, &Property::frontfaceCullingChanged, &vtkProperty::SetFrontfaceCulling);           }
void Property::setSpecularColor   (QColor                   v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_specularColor,    &Property::specularColorChanged,    &vtkProperty::SetSpecularColor, qcolor2vtk);  }
void Property::setDiffuseColor    (QColor                   v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_diffuseColor,     &Property::diffuseColorChanged,     &vtkProperty::SetSpecularColor, qcolor2vtk);  }
void Property::setAmbientColor    (QColor                   v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_ambientColor,     &Property::ambientColorChanged,     &vtkProperty::SetSpecularColor, qcolor2vtk);  }
void Property::setEdgeColor       (QColor                   v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_edgeColor,        &Property::edgeColorChanged,        &vtkProperty::SetSpecularColor, qcolor2vtk);  }
void Property::setAmbient         (qreal                    v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_ambient,          &Property::ambientChanged,          &vtkProperty::SetAmbient);                    }
void Property::setDiffuse         (qreal                    v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_diffuse,          &Property::diffuseChanged,          &vtkProperty::SetDiffuse);                    }
void Property::setSpecular        (qreal                    v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_specular,         &Property::specularChanged,         &vtkProperty::SetSpecular);                   }
void Property::setSpecularPower   (qreal                    v, bool force) { QProperty_setter_impl(v, force, this, &Property::m_specularPower,    &Property::specularPowerChanged,    &vtkProperty::SetSpecularPower);              }


Property::vtkUserData Property::initializeVTK(vtkRenderWindow*, vtkUserData renderData)
{
    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->property = vtkProperty::New();

    m_vtkInitialized = true;

    setInterpolation    (m_interpolation,    true);
    setRepresentation   (m_representation,   true);
    setOpacity          (m_opacity,          true);
    setLineWidth        (m_lineWidth,        true);
    setPointSize        (m_pointSize,        true);
    setShading          (m_shading,          true);
    setLighting         (m_lighting,         true);
    setEdgeVisibility   (m_edgeVisibility,   true);
    setBackfaceCulling  (m_backfaceCulling,  true);
    setFrontfaceCulling (m_frontfaceCulling, true);
    setSpecularColor    (m_specularColor,    true);
    setDiffuseColor     (m_diffuseColor,     true);
    setAmbientColor     (m_ambientColor,     true);
    setEdgeColor        (m_edgeColor,        true);
    setAmbient          (m_ambient,          true);
    setSpecular         (m_specular,         true);
    setSpecularPower    (m_specularPower,    true);

    return vtk;
}

vtkProperty* Property::myVtkObject(vtkUserData renderData) const
{
    auto vtk = MyVtkData::SafeDownCast(renderData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(renderData) FAILED";
        return {};
    }

    return vtk->property;
}

bool Property::isVolatile() const
{
    return true;
}

} }
