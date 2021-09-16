#include "quickVtkAlgorithm.h"

#include "QQmlListProperty_impl.h"

#include <vtkAlgorithm.h>

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<Algorithm>
{
    vtkSmartPointer<vtkAlgorithm> algorithm;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

Algorithm::Algorithm(QObject* parent) : Object(Object::Type::Algorithm, parent)
{}

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

Algorithm::vtkUserData Algorithm::initializeVTK(WeakDispatcherPtr weakDispatcher, vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    qDebug() << Q_FUNC_INFO << m_vtkInitialized;

    auto vtk = vtkNew<MyVtkData>(this, weakDispatcher, renderData);

    vtk->algorithm = makeAlgorithm();

    for(int index=0; index<m_input.count(); ++index) {
        auto& object = m_input.at(index);
        inputImpl.attachObject(this, object, index, renderWindow, renderData);
    }

    m_vtkInitialized = true;

    return vtk;

}
vtkAlgorithm* Algorithm::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!!" << Q_FUNC_INFO << "MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->algorithm;
}

} }
