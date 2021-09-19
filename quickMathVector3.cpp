#include "quickMathVector3.h"

namespace quick { namespace math {

Vector3::Vector3(qreal x, qreal y, qreal z, QObject* parent) : QObject(parent), m_values{x,y,z}
{}

double Vector3::x() const { return m_values[0]; }
double Vector3::y() const { return m_values[1]; }
double Vector3::z() const { return m_values[2]; }

void Vector3::setX(double v) { if (m_values[0] != v) emit xChanged(m_values[0] = v); }
void Vector3::setY(double v) { if (m_values[1] != v) emit xChanged(m_values[1] = v); }
void Vector3::setZ(double v) { if (m_values[2] != v) emit xChanged(m_values[2] = v); }

} }
