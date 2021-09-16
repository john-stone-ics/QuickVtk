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
    void setVisibility(bool);
    Q_SIGNAL void visibilityChanged(bool);

protected:
    Prop(QObject* parent);

private:
    bool m_visibility = false;
};

} }
