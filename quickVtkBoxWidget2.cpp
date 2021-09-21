#include "quickVtkBoxWidget2.h"

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<BoxWidget2>
{
    vtkSmartPointer<vtkBoxWidget2> boxWidget2;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

BoxWidget2::BoxWidget2(QObject* parent) : AbstractWidget(parent)
{}

BoxWidget2::vtkUserData BoxWidget2::initializeVTK(vtkRenderWindow*, vtkUserData renderData)
{
    qDebug() << m_vtkInitialized;

    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->boxWidget2 = vtkBoxWidget2::New();

    m_vtkInitialized = true;

    return vtk;
}

vtkBoxWidget2* BoxWidget2::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->boxWidget2;
}

} }
