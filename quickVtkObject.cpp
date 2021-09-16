#include "quickVtkObject.h"

namespace quick { namespace vtk {

Object::Object(Type type, QObject* parent) : QObject(parent), m_type(type), m_weakDispatcher(nullptr)
{}

Object::Type Object::type() const
{
    return m_type;
}

} }
