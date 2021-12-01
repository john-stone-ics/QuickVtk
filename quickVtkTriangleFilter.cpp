#include "quickVtkTriangleFilter.h"
#include "QProperty_setter_impl.h"

namespace quick { namespace vtk {

TriangleFilter::TriangleFilter(QObject* parent) : PolyDataAlgorithm(parent)
{}

bool TriangleFilter::passVerts() const { return m_passVerts; }
bool TriangleFilter::passLines() const { return m_passLines; }

void TriangleFilter::setPassVerts(bool v, bool force) { QProperty_setter_impl(v, force, this, &TriangleFilter::m_passVerts, &TriangleFilter::passVertsChanged, &vtkTriangleFilter::SetPassVerts); }
void TriangleFilter::setPassLines(bool v, bool force) { QProperty_setter_impl(v, force, this, &TriangleFilter::m_passLines, &TriangleFilter::passLinesChanged, &vtkTriangleFilter::SetPassLines); }

TriangleFilter::vtkUserData TriangleFilter::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto myUserData = PolyDataAlgorithm::initializeVTK(renderWindow, renderData);

    setPassVerts(m_passVerts, true);
    setPassLines(m_passLines, true);

    return myUserData;
}
vtkTriangleFilter* TriangleFilter::myVtkObject(Object::vtkUserData myUserData) const
{
    auto vtkTriangleFilter = vtkTriangleFilter::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData));

    if (!vtkTriangleFilter)
        qWarning() << "YIKES!! vtkTriangleFilter::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData)) FAILED";

    return vtkTriangleFilter;
}

vtkTriangleFilter* TriangleFilter::makeAlgorithm()
{
    return vtkTriangleFilter::New();
}

} }
