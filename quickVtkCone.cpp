#include "quickVtkCone.h"
#include "quickVtkDispatcher.h"
#include "QProperty_setter_impl.h"

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<Cone>
{
    vtkSmartPointer<vtkCone> cone;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

Cone::Cone(QObject* parent) : ImplicitFunction(parent)
{}

qreal Cone::angle() const
{
    return m_angle;
}

void Cone::setAngle(qreal v, bool force)
{
    QProperty_setter_impl(v, force, this, &Cone::m_angle, &Cone::angleChanged, &vtkCone::SetAngle);
}

Cone::vtkUserData Cone::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    qDebug() << m_vtkInitialized;

    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->cone = vtkCone::New();

    m_vtkInitialized = true;

    setAngle(m_angle, true);

    return vtk;
}

vtkCone* Cone::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->cone;
}

bool Cone::isVolatile() const
{
    return true;
}

} }
