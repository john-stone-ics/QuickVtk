#pragma once

#include "quickVtkAlgorithm.h"

#include <vtkAbstractMapper.h>

namespace quick { namespace vtk {

class AbstractMapper : public Algorithm
{
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "input");
public:

    // input property
    Q_PROPERTY(QQmlListProperty<quick::vtk::Algorithm> input READ input NOTIFY inputChanged);
    QQmlListProperty<Algorithm> input();
    Q_SIGNAL void inputChanged();
    QList<Algorithm*> m_input;

protected:
    AbstractMapper(QObject* parent);

public:
    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkAbstractMapper* myVtkObject(vtkUserData) const override;
    bool isVolatile() const override;

private:
    vtkAlgorithm* makeAlgorithm() override;

    virtual vtkAbstractMapper* makeAbstractMapper() = 0;
};

} }
