#pragma once

#include "quickVtkObject.h"

#include <vtkAlgorithm.h>

#include <QtQml/QQmlListProperty>

namespace quick { namespace vtk {

class Algorithm : public Object
{
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "input");
public:

    // input property
    Q_PROPERTY(QQmlListProperty<quick::vtk::Algorithm> input READ input NOTIFY inputChanged);
    QQmlListProperty<Algorithm> input();
    Q_SIGNAL void inputChanged();
    QList<Algorithm*> m_input;

    vtkUserData initializeVTK(WeakDispatcherPtr, vtkRenderWindow*, vtkUserData) override;
    vtkAlgorithm* myVtkObject(vtkUserData) const override;

protected:
    Algorithm(QObject* parent);

private:
    virtual vtkAlgorithm* makeAlgorithm() = 0;
};

} }
