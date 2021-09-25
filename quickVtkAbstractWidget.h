#pragma once

#include "quickVtkObject.h"

#include <vtkAbstractWidget.h>

namespace quick { namespace vtk {

class AbstractWidget : public Object
{
    Q_OBJECT
public:

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    bool enabled() const;
    void setEnabled(bool, bool=false);
    Q_SIGNAL void enabledChanged(bool);
    bool m_enabled = true;

protected:
    AbstractWidget(QObject* parent);
    virtual vtkAbstractWidget* makeAbstractWidget() const = 0;

public:
    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkAbstractWidget* myVtkObject(vtkUserData) const override;
    bool isVolatile() const override;
};

} }
