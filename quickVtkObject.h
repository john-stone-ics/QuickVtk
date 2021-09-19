#pragma once

#include "quickVtkDispatcher.h"

#include <vtkObject.h>
#include <vtkSmartPointer.h>
class vtkRenderWindow;

#include <QtCore/QObject>

namespace quick { namespace vtk {

class Object : public QObject, public SharedData
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
    virtual vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) = 0;
    virtual vtkObject* myVtkObject(vtkUserData) const = 0;
    virtual bool isVolatile() const = 0;

private:
    Type m_type;
};

} }
