#include "quickVtkProp.h"

namespace quick { namespace vtk {

Prop::Prop(QObject* parent) : Object(Object::Type::Prop, parent)
{}

bool Prop::visibility() const
{
    return m_visibility;
}
void Prop::setVisibility(bool v)
{
    if (m_visibility != v)
        emit visibilityChanged(m_visibility = v);
}

} }
