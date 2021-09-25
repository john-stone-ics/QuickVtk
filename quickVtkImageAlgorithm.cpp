#include "quickVtkImageAlgorithm.h"

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<ImageAlgorithm>
{
    vtkSmartPointer<vtkImageAlgorithm> imageAlgorithm;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

ImageAlgorithm::ImageAlgorithm(QObject* parent) : Algorithm(parent)
{}

ImageAlgorithm::vtkUserData ImageAlgorithm::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->imageAlgorithm = vtkImageAlgorithm::SafeDownCast(makeAlgorithm());

    if (!vtk->imageAlgorithm)
        qWarning() << "YIKES!! vtkImageAlgorithm::SafeDownCast(makeAlgorithm()) FAILED";

    m_vtkInitialized = true;

    auto shouldBeNullptr = Algorithm::initializeVTK(renderWindow, renderData);

    if (shouldBeNullptr)
        qWarning() << "YIKES!! Algorithm::initializeVTK(renderWindow, renderData) returned non-nullptr";

    return vtk;
}

vtkImageAlgorithm* ImageAlgorithm::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->imageAlgorithm;
}

bool ImageAlgorithm::isVolatile() const
{
    return true;
}

} }
