#include "quickVtkBooleanOperationPolyDataFilter.h"
#include "QProperty_setter_impl.h"

namespace quick { namespace vtk {

BooleanOperationPolyDataFilter::BooleanOperationPolyDataFilter(QObject* parent) : PolyDataAlgorithm(parent)
{}

BooleanOperationPolyDataFilter::Operation BooleanOperationPolyDataFilter::operation             () const { return m_operation; }
qreal                                     BooleanOperationPolyDataFilter::tolerance             () const { return m_tolerance; }
bool                                      BooleanOperationPolyDataFilter::reorientDifferentCells() const { return m_reorientDifferentCellsChanged; }

void BooleanOperationPolyDataFilter::setReorientDifferentCells(bool v, bool force) { QProperty_setter_impl(v, force, this, &BooleanOperationPolyDataFilter::m_reorientDifferentCellsChanged, &BooleanOperationPolyDataFilter::reorientDifferentCellsChanged, &vtkBooleanOperationPolyDataFilter::SetReorientDifferenceCells); }
void BooleanOperationPolyDataFilter::setOperation(Operation         v, bool force) { QProperty_setter_impl(v, force, this, &BooleanOperationPolyDataFilter::m_operation,                     &BooleanOperationPolyDataFilter::operationChanged,              &vtkBooleanOperationPolyDataFilter::SetOperation              ); }
void BooleanOperationPolyDataFilter::setTolerance(qreal             v, bool force) { QProperty_setter_impl(v, force, this, &BooleanOperationPolyDataFilter::m_tolerance,                     &BooleanOperationPolyDataFilter::toleranceChanged,              &vtkBooleanOperationPolyDataFilter::SetTolerance              ); }

BooleanOperationPolyDataFilter::vtkUserData BooleanOperationPolyDataFilter::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    auto myUserData = PolyDataAlgorithm::initializeVTK(renderWindow, renderData);

    setReorientDifferentCells(m_reorientDifferentCellsChanged, true);
    setOperation(m_operation, true);
    setTolerance(m_tolerance, true);

    return myUserData;
}
vtkBooleanOperationPolyDataFilter* BooleanOperationPolyDataFilter::myVtkObject(Object::vtkUserData myUserData) const
{
    auto vtkBooleanOperationPolyDataFilter = vtkBooleanOperationPolyDataFilter::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData));

    if (!vtkBooleanOperationPolyDataFilter)
        qWarning() << "YIKES!! vtkBooleanOperationPolyDataFilter::SafeDownCast(PolyDataAlgorithm::myVtkObject(myUserData)) FAILED";

    return vtkBooleanOperationPolyDataFilter;
}

vtkBooleanOperationPolyDataFilter* BooleanOperationPolyDataFilter::makeAlgorithm()
{
    return vtkBooleanOperationPolyDataFilter::New();
}

} }
