#pragma once

#include "quickVtkPolyDataAlgorithm.h"

#include <vtkTriangleFilter.h>

class vtkTriangleFilter;

namespace quick { namespace vtk {

class TriangleFilter : public PolyDataAlgorithm {
    Q_OBJECT
public:
    TriangleFilter(QObject* parent = nullptr) ;

    Q_PROPERTY(bool passVerts READ passVerts WRITE setPassVerts NOTIFY passVertsChanged)
    bool passVerts() const;
    void setPassVerts(bool, bool=false);
    Q_SIGNAL void passVertsChanged(bool);
    bool m_passVerts = false;

    Q_PROPERTY(bool passLines READ passLines WRITE setPassLines NOTIFY passLinesChanged)
    bool passLines() const;
    void setPassLines(bool, bool=false);
    Q_SIGNAL void passLinesChanged(bool);
    bool m_passLines = false;

public:
    vtkUserData initializeVTK(vtkRenderWindow*, vtkUserData) override;
    vtkTriangleFilter* myVtkObject(Object::vtkUserData) const override;
private:
    vtkTriangleFilter* makeAlgorithm() override;
};

} }
