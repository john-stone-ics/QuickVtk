#include "quickVtkAbstractMapper.h"

#include "QQmlListProperty_impl.h"

#include <vtkAbstractMapper.h>

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<AbstractMapper>
{
    ~MyVtkData() { qDebug() << qobj; }

    vtkSmartPointer<vtkAbstractMapper> abstractMapper;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

AbstractMapper::AbstractMapper(QObject* parent) : Algorithm(parent)
{}

namespace {
void attachToObject(vtkSmartPointer<vtkAbstractMapper> parent, vtkSmartPointer<vtkAlgorithm> child, int index)
{
    parent->SetInputConnection(index, child->GetOutputPort());
}

void detachFromObject(vtkSmartPointer<vtkAbstractMapper> parent, vtkSmartPointer<vtkAlgorithm> child, int index)
{
    //TODO: figure out how to verify the existing connection here

    parent->RemoveInputConnection(index, child->GetOutputPort());
}

QQmlListProperty_impl<AbstractMapper, vtkAbstractMapper, Algorithm, vtkAlgorithm, attachToObject, detachFromObject> inputImpl;
}

QQmlListProperty<Algorithm> AbstractMapper::input()
{
    return QQmlListProperty<Algorithm>(this, &m_input, inputImpl.append, inputImpl.count, inputImpl.at, inputImpl.clear);
}

AbstractMapper::vtkUserData AbstractMapper::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    qDebug() << this << m_vtkInitialized;

    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->abstractMapper = makeAbstractMapper();

    for(int index=0; index<m_input.count(); ++index) {
        auto& object = m_input.at(index);
        inputImpl.attachObject(this, object, index, renderWindow, renderData);
    }

    m_vtkInitialized = true;

    return vtk;
}

vtkAbstractMapper* AbstractMapper::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->abstractMapper;
}

vtkAlgorithm* AbstractMapper::makeAlgorithm()
{
    qFatal("YIKES!! %s makeAlgorithm() should never be called", Q_FUNC_INFO);
    return nullptr;
}

bool AbstractMapper::isVolatile() const
{
    return true;
}

} }
