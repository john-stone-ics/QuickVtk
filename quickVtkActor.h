#pragma once

#include "quickVtkProp3D.h"
#include "quickVtkProperty.h"

#include <vtkActor.h>

namespace quick { namespace vtk {

class Dispatcher;
class Mapper;
class Texture;

class Actor : public Prop3D {
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "mapper");
public:

    //Q_PROPERTY(quick::Vtk::Texture* texture READ getTexture CONSTANT);

    Q_PROPERTY(quick::vtk::Mapper* mapper READ mapper WRITE setMapper NOTIFY mapperChanged);
    Mapper* mapper() const;
    void setMapper(Mapper*);
    Q_SIGNAL void mapperChanged(quick::vtk::Mapper*);
    Mapper* m_mapper = nullptr;

    Q_PROPERTY(quick::vtk::Property* property READ property CONSTANT);
    Property* property();
    Property m_property;

public:
    Actor(QObject* parent=nullptr);
    ~Actor() override;

    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkActor* myVtkObject(vtkUserData) const override;

protected:
    vtkActor* makeProp() const override;
};

} }
