#include "quickVtkAbstractWidget.h"
#include "QProperty_setter_impl.h"

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<AbstractWidget>
{
    vtkSmartPointer<vtkAbstractWidget> abstractWidget;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

AbstractWidget::AbstractWidget(QObject* parent) : Object(Object::Type::Widget, parent)
{}

bool AbstractWidget::enabled() const
{
    return m_enabled;
}

void AbstractWidget::setEnabled(bool v, bool force)
{
    QProperty_setter_impl(v, force, this, &AbstractWidget::m_enabled, &AbstractWidget::enabledChanged, &vtkAbstractWidget::SetEnabled);
}

AbstractWidget::vtkUserData AbstractWidget::initializeVTK(vtkRenderWindow*, vtkUserData renderData)
{
    auto vtk = vtkNew<MyVtkData>(this, renderData);

    vtk->abstractWidget = makeAbstractWidget();

    m_vtkInitialized = true;

    setEnabled(m_enabled, true);

    return vtk;
}

vtkAbstractWidget* AbstractWidget::myVtkObject(vtkUserData myUserData) const
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(myUserData) FAILED";
        return {};
    }

    return vtk->abstractWidget;
}

bool AbstractWidget::isVolatile() const
{
    return true;
}

} }
