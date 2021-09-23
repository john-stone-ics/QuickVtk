#pragma once

#include "quickVtkAbstractWidget.h"

#include <vtkBoxWidget2.h>

namespace quick { namespace vtk {

class BoxWidget2 : public AbstractWidget
{
    Q_OBJECT
public:
    BoxWidget2(QObject* parent = nullptr);

    vtkBoxWidget2* myVtkObject(vtkUserData) const override;
protected:
    vtkBoxWidget2 *makeAbstractWidget() const override;

};

} }
