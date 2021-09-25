#pragma once

#include "quickVtkPolyDataAlgorithm.h"

#include <vtkBooleanOperationPolyDataFilter.h>

class vtkBooleanOperationPolyDataFilter;

namespace quick { namespace vtk {

class BooleanOperationPolyDataFilter : public PolyDataAlgorithm {
    Q_OBJECT
public:
    BooleanOperationPolyDataFilter(QObject* parent = nullptr) ;

    enum Operation {
        Union = vtkBooleanOperationPolyDataFilter::VTK_UNION,
        Intersection = vtkBooleanOperationPolyDataFilter::VTK_INTERSECTION,
        Difference = vtkBooleanOperationPolyDataFilter::VTK_DIFFERENCE
    };
    Q_ENUM(Operation);
    Q_PROPERTY(quick::vtk::BooleanOperationPolyDataFilter::Operation operation READ operation WRITE setOperation NOTIFY operationChanged)
    Operation operation() const;
    void setOperation(Operation, bool=false);
    Q_SIGNAL void operationChanged(Operation);
    Operation m_operation = Union;

    Q_PROPERTY(qreal tolerance READ tolerance WRITE setTolerance NOTIFY toleranceChanged)
    qreal tolerance() const;
    void setTolerance(qreal, bool=false);
    Q_SIGNAL void toleranceChanged(qreal);
    qreal m_tolerance = 1.0;

    Q_PROPERTY(bool reorientDifferentCells READ reorientDifferentCells WRITE setReorientDifferentCells NOTIFY reorientDifferentCellsChanged)
    bool reorientDifferentCells() const;
    void setReorientDifferentCells(bool, bool=false);
    Q_SIGNAL void reorientDifferentCellsChanged(bool);
    bool m_reorientDifferentCellsChanged = false;

public:
    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkBooleanOperationPolyDataFilter* myVtkObject(Object::vtkUserData=nullptr) const override;
private:
    vtkBooleanOperationPolyDataFilter* makeAlgorithm() override;
};

} }
