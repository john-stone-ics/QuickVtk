#pragma once

#include "quickVtkDispatcher.h"
#include "quickVtkObject.h"

class vtkRenderer;

#include <QtQml/QQmlListProperty>

#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QtCore/QAtomicInteger>
#include <QtCore/QAtomicPointer>

#include <QQuickVtkItem.h>

namespace quick { namespace vtk {

class Viewer : public QQuickVtkItem, public Dispatcher, public SharedData
{
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "input");
public:
    Viewer(QQuickItem* parent=nullptr);

    // QQuickVtkItem interface
    using vtkUserData = vtkSmartPointer<vtkObject>;
    vtkUserData initializeVTK(vtkRenderWindow*) override;

    // Dispatcher interface
    void dispatch_async(std::function<void(vtkRenderWindow*, vtkUserData)>&&) override;
    bool map(QObject*, vtkUserData, vtkUserData) override;
    bool unmap(QObject*, vtkUserData) override;
    vtkUserData lookup(QObject*, vtkUserData, bool mightBeEmpty=false) override;
    vtkRenderer* myVtkObject(vtkUserData);

    // input property
    Q_PROPERTY(QQmlListProperty<quick::vtk::Object> input READ input NOTIFY inputChanged);
    QQmlListProperty<Object> input();
    Q_SIGNAL void inputChanged();
    QList<Object*> m_input;

signals:
    void clicked(QMouseEvent*);

private:
    bool event(QEvent*) override;

private:
    struct {
        Qt::MouseButton button = Qt::NoButton;
        QPointF localPos;
    } m_click;
};

}}
