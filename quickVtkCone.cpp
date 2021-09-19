#include "quickVtkCone.h"
#include "quickVtkDispatcher.h"

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

void Cone::setAngle(qreal v)
{
    if (m_angle != v) {
        emit angleChanged(m_angle = v);

        if (!m_vtkInitialized)
            return;

        dispatcher()->dispatch_async([
             pThis = QPointer<Cone>(this),
             angle = v]
        (vtkRenderWindow* renderWindow, vtkUserData renderData) mutable
        {
            auto dispatcher = pThis->dispatcher();

            if (!dispatcher) {
                qWarning() << "YIKES!! pThis->dispatcher() FAILED";
                return;
            }

            auto myUserData = dispatcher->lookup(pThis, renderData);

            if (!myUserData) {
                return;
            }

            auto vtkCone = pThis->myVtkObject(myUserData);

            if (!vtkCone) {
                return;
            }

            vtkCone->SetAngle(angle);
        });
    }
}


Cone::vtkUserData Cone::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    qDebug() << m_vtkInitialized;

    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->cone = vtkCone::New();

    m_vtkInitialized = true;

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
