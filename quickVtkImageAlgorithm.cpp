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

    vtk->imageAlgorithm = makeImageAlgorithm();

    m_vtkInitialized = true;

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

vtkAlgorithm* ImageAlgorithm::makeAlgorithm()
{
    qFatal("YIKES!! %s makeAlgorithm() should never be called", Q_FUNC_INFO);
    return nullptr;
}

bool ImageAlgorithm::isVolatile() const
{
    return true;
}

} }
