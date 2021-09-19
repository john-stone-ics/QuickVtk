#pragma once

#include "quickVtkAbstractMapper3D.h"

#include <vtkMapper.h>

namespace quick { namespace vtk {

class Mapper : public AbstractMapper3D
{
    Q_OBJECT
public:

    enum class ScalarMode {
        ScalarModeDefault           = VTK_SCALAR_MODE_DEFAULT,
        ScalarModeUsePointData      = VTK_SCALAR_MODE_USE_POINT_DATA,
        ScalarModeUseCellData       = VTK_SCALAR_MODE_USE_CELL_DATA,
        ScalarModeUsePointFieldData = VTK_SCALAR_MODE_USE_POINT_FIELD_DATA,
        ScalarModeUseCellFieldData  = VTK_SCALAR_MODE_USE_CELL_FIELD_DATA
    };
    Q_ENUM(ScalarMode);
    Q_PROPERTY(quick::vtk::Mapper::ScalarMode scalarMode READ scalarMode WRITE setScalarMode NOTIFY scalarModeChanged)
    ScalarMode scalarMode() const;
    void setScalarMode(enum ScalarMode, bool force=false);
    Q_SIGNAL void scalarModeChanged(enum ScalarMode);
    enum ScalarMode m_scalarMode = ScalarMode::ScalarModeDefault;

    Q_PROPERTY(bool scalarVisibility READ scalarVisibility WRITE setScalarVisibility NOTIFY scalarVisibilityChanged)
    bool scalarVisibility() const;
    void setScalarVisibility(bool, bool force=false);
    Q_SIGNAL void scalarVisibilityChanged(bool);
    bool m_scalarVisibility = false;

protected:
    Mapper(QObject* parent);

public:
    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkMapper* myVtkObject(Object::vtkUserData) const override;
};

} }
