#pragma once

#include "quickVtkProp3D.h"

#include <vtkActor.h>

namespace quick { namespace vtk {

class Dispatcher;
class Mapper;
class Texture;
class Property;

class Actor : public Prop3D {
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "mapper");
public:

    //Q_PROPERTY(quick::Vtk::Texture* texture READ getTexture CONSTANT);
    //Q_PROPERTY(quick::Vtk::Property* property READ getProperty CONSTANT);

    Q_PROPERTY(quick::vtk::Mapper* mapper READ mapper WRITE setMapper NOTIFY mapperChanged);
    Mapper* mapper() const;
    void setMapper(Mapper*);
    Q_SIGNAL void mapperChanged(quick::vtk::Mapper*);
    Mapper* m_mapper = nullptr;

public:
    Actor(QObject* parent=nullptr);

    vtkUserData initializeVTK(WeakDispatcherPtr, vtkRenderWindow*, vtkUserData) override;
    vtkActor* myVtkObject(vtkUserData) const override;
};

} }
