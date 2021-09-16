#pragma once

#include "quickVtkDispatcher.h"

#include <vtkObject.h>
#include <vtkSmartPointer.h>
class vtkRenderWindow;

#include <QtCore/QObject>
#include <QtCore/QAtomicInt>

namespace quick { namespace vtk {

class Object : public QObject
{
    Q_OBJECT
public:
    enum class Type {
        Prop,
        Algorithm,
        Widget,
        Other
    };
    Object(Type, QObject* parent);
    Type type() const;

    using vtkUserData = vtkSmartPointer<vtkObject>;
    virtual vtkUserData initializeVTK(WeakDispatcherPtr, vtkRenderWindow*, vtkUserData) = 0;
    virtual vtkObject* myVtkObject(vtkUserData) const = 0;

private:
    Type m_type;

public:
    WeakDispatcherPtr m_weakDispatcher;

    QAtomicInt m_vtkInitialized = 0;
};

} }
