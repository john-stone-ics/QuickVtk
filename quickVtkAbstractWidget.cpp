#include "quickVtkAbstractWidget.h"

namespace quick { namespace vtk {

AbstractWidget::AbstractWidget(QObject* parent) : Object(Object::Type::Widget, parent)
{}

bool AbstractWidget::enabled() const
{
    return m_enabled;
}
void AbstractWidget::setEnabled(bool v)
{
    if (m_enabled != v)
        emit enabledChanged(m_enabled = v);
}

bool AbstractWidget::isVolatile() const
{
    return true;
}

} }
