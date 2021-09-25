#include "quickVtkAlgorithm.h"
#include "QQmlListProperty_impl.h"

#include <vtkAlgorithm.h>

namespace quick { namespace vtk {

Algorithm::Algorithm(QObject* parent) : Object(Object::Type::Algorithm, parent)
{
}

namespace {
void attachToObject(vtkSmartPointer<vtkAlgorithm> parent, vtkSmartPointer<vtkAlgorithm> child, int index)
{
    parent->SetInputConnection(index, child->GetOutputPort());
}

void detachFromObject(vtkSmartPointer<vtkAlgorithm> parent, vtkSmartPointer<vtkAlgorithm> child, int index)
{
    //TODO: figure out how to verify the existing connection here

    parent->RemoveInputConnection(index, child->GetOutputPort());
}

QQmlListProperty_impl<Algorithm, vtkAlgorithm, Algorithm, vtkAlgorithm, attachToObject, detachFromObject> inputImpl;
}

QQmlListProperty<Algorithm> Algorithm::input()
{
    return QQmlListProperty<Algorithm>(this, &m_input, inputImpl.append, inputImpl.count, inputImpl.at, inputImpl.clear);
}

Algorithm::vtkUserData Algorithm::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    if (!m_vtkInitialized)
    {
        m_vtkAlgorithm = makeAlgorithm();
        m_vtkInitialized = true;
    }

    for(int index=0; index<m_input.count(); ++index) {
        auto& object = m_input.at(index);
        inputImpl.attachObject(this, object, index, renderWindow, renderData);
    }

    return nullptr;
}

vtkAlgorithm* Algorithm::myVtkObject(vtkUserData myUserData) const
{
    if (myUserData)
        qWarning() << "YIKES!! is non-volitile but was called with non-null myUserData";

    return m_vtkAlgorithm;
}

bool Algorithm::isVolatile() const
{
    return false;
}

} }
