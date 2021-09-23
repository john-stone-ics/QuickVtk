#include "quickVtkBoxWidget2.h"

namespace quick { namespace vtk {

BoxWidget2::BoxWidget2(QObject* parent) : AbstractWidget(parent)
{}

vtkBoxWidget2* BoxWidget2::myVtkObject(vtkUserData myUserData) const
{
    auto vtkBoxWidget2 = vtkBoxWidget2::SafeDownCast(AbstractWidget::myVtkObject(myUserData));

    if (!vtkBoxWidget2)
        qWarning() << "YIKES!! vtkBoxWidget2::SafeDownCast(AbstractWidget::myVtkObject(myUserData)) FAILED";

    return vtkBoxWidget2;
}

vtkBoxWidget2* BoxWidget2::makeAbstractWidget() const
{
    return vtkBoxWidget2::New();
}

} }
