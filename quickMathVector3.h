#pragma once

#include <QtCore/QObject>

#include <array>

namespace quick { namespace math {

class Vector3 : public QObject {
    Q_OBJECT
public:
    Vector3(qreal x=0, qreal y=0, qreal z=0, QObject* parent = nullptr);

    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged);
    qreal x() const;
    void setX(qreal);
    Q_SIGNAL void xChanged(qreal);

    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged);
    qreal y() const;
    void setY(qreal);
    Q_SIGNAL void yChanged(qreal);

    Q_PROPERTY(qreal z READ z WRITE setZ NOTIFY zChanged);
    qreal z() const;
    void setZ(qreal);
    Q_SIGNAL void zChanged(qreal);

    std::array<qreal, 3> m_values;
};

} }
