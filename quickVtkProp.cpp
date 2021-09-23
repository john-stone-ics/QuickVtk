#include "quickVtkProp.h"
#include "QProperty_setter_impl.h"

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<Prop>
{
    ~MyVtkData() { qDebug() << qobj; }

    vtkSmartPointer<vtkProp> prop;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

Prop::Prop(QObject* parent) : Object(Object::Type::Prop, parent)
{}

bool Prop::visibility() const
{
    return m_visibility;
}

void Prop::setVisibility(bool v, bool force)
{
    QProperty_setter_impl(v, force, this, &Prop::m_visibility, &Prop::visibilityChanged, &vtkProp::SetVisibility);
}

Prop::vtkUserData Prop::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->prop = makeProp();

    m_vtkInitialized = true;

    return vtk;
}

vtkProp* Prop::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->prop;
}

bool Prop::isVolatile() const
{
    return true;
}

} }
