#include "quickVtkAbstractMapper.h"

#include "QQmlListProperty_impl.h"

#include <vtkAbstractMapper.h>

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<AbstractMapper>
{
    vtkSmartPointer<vtkAbstractMapper> abstractMapper;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

AbstractMapper::AbstractMapper(QObject* parent) : Algorithm(parent)
{}

AbstractMapper::vtkUserData AbstractMapper::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->abstractMapper = vtkAbstractMapper::SafeDownCast(makeAlgorithm());

    if (!vtk->abstractMapper)
        qWarning() << "YIKES!! vtkAbstractMapper::SafeDownCast(makeAlgorithm()) FAILED";

    m_vtkInitialized = true;

    auto shouldBeNullptr = Algorithm::initializeVTK(renderWindow, renderData);

    if (shouldBeNullptr)
        qWarning() << "YIKES!! Algorithm::initializeVTK(renderWindow, renderData) returned non-nullptr";

    return vtk;
}

vtkAbstractMapper* AbstractMapper::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->abstractMapper;
}

bool AbstractMapper::isVolatile() const
{
    return true;
}

} }
