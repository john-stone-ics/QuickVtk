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
    void setEnabled(bool);
    Q_SIGNAL void enabledChanged(bool);
    bool m_enabled = false;

protected:
    AbstractWidget(QObject* parent);

public:
    bool isVolatile() const;
};

} }
