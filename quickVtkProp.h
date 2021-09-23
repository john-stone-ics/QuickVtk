#pragma once

#include "quickVtkObject.h"

#include <vtkProp.h>

namespace quick { namespace vtk {

class Prop : public Object
{
    Q_OBJECT
public:

    Q_PROPERTY(bool visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)
    bool visibility() const;
    void setVisibility(bool, bool=false);
    Q_SIGNAL void visibilityChanged(bool);

    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkProp* myVtkObject(vtkUserData) const override;
    bool isVolatile() const override;

protected:
    Prop(QObject* parent);
    virtual vtkProp* makeProp() const = 0;

private:
    bool m_visibility = false;
};

} }
