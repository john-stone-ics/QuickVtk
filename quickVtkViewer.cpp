#include "quickVtkViewer.h"

#include "QQmlListProperty_impl.h"

#include <vtkProp.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkAbstractWidget.h>

#include <QtQuick/QQuickWindow>

#include <QtCore/QPointer>
#include <QtCore/QDebug>
#include <QtCore/QMutex>

#include <sstream>

namespace quick { namespace vtk {

namespace {
struct MyVtkData : UserData<Viewer>
{
    vtkSmartPointer<vtkRenderer> renderer;

    QMap<QObject*, Object::vtkUserData> map;

    static MyVtkData* New() { return new MyVtkData; }
    vtkTypeMacro(MyVtkData, vtkObject);
};
}

Viewer::Viewer(QQuickItem* parent) : QQuickVtkItem(parent)
{}

Viewer::~Viewer()
{
    aboutToBeDeleted();
}

bool Viewer::map(QObject * object, vtkUserData objectData, vtkUserData myVtkData)
{
    if (object == this)
        return true;

    auto vtk = MyVtkData::SafeDownCast(myVtkData);

    if (!vtk) {
        qWarning() << "YIKES!! myVtkData::SafeDownCast(myVtkData) FAILED";
        return false;
    }

    if (vtk->map.contains(object)) {
        qWarning() << "YIKES!! object already mapped" << object;
        return false;
    }

    vtk->map.insert(object, objectData);
    return true;
}

Object::vtkUserData Viewer::lookup(QObject * object, vtkUserData renderData, bool mightBeEmpty)
{
    auto vtk = MyVtkData::SafeDownCast(renderData);

    if (!vtk) {
        qWarning() << "YIKES!! myVtkData::SafeDownCast(myVtkData) FAILED";
        return {};
    }

    if (object == this)
        return vtk;

    if (!vtk->map.contains(object)) {
        if (!mightBeEmpty)
            qWarning() << "YIKES!! object not mapped" << object;
        return {};
    }

    return vtk->map.value(object);
}

bool Viewer::unmap(QObject* object, vtkUserData myVtkData)
{
    if (object == this)
        return true;

    auto vtk = MyVtkData::SafeDownCast(myVtkData);

    if (!vtk) {
        qWarning() << "YIKES!! myVtkData::SafeDownCast(myVtkData) FAILED";
        return false;
    }

    if (!vtk->map.contains(object)) {
        qWarning() << "YIKES!! object is not mapped" << object;
        return false;
    }

    return vtk->map.remove(object) > 0;
}

namespace {
void attachToObject(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkObject> objVtkObject, int index)
{
    if (auto vtkProp = vtkProp::SafeDownCast(objVtkObject)) {
        renderer->AddActor(vtkProp);
    }

    else if (auto vtkAbstractWidget = vtkAbstractWidget::SafeDownCast(objVtkObject)) {
        vtkAbstractWidget->SetInteractor(renderer->GetRenderWindow()->GetInteractor());
        vtkAbstractWidget->CreateDefaultRepresentation();
        vtkAbstractWidget->On();
    }

    else {
        std::stringstream ss; objVtkObject->Print(ss);
        qWarning() << "YIKES!! Attempt to attach unknown typed object to quick::vtk::Viewer ignored" << ss.str().c_str();
    }
}

void detachFromObject(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkObject> objVtkObject, int index)
{
    if (auto vtkProp = vtkProp::SafeDownCast(objVtkObject)) {
        renderer->RemoveActor(vtkProp);
    }
}

QQmlListProperty_impl<Viewer, vtkRenderer, Object, vtkObject, attachToObject, detachFromObject> inputImpl;
}

QQmlListProperty<Object> Viewer::input()
{
    return QQmlListProperty(this, &m_input, inputImpl.append, inputImpl.count, inputImpl.at, inputImpl.clear);
}

Viewer::vtkUserData Viewer::initializeVTK(vtkRenderWindow* renderWindow)
{
    auto vtk = vtkNew<MyVtkData>(this);

    vtk->renderer = vtkRenderer::New();

    renderWindow->AddRenderer(vtk->renderer);

    for(int index=0; index<m_input.count(); ++index) {
        auto& object = m_input.at(index);
        inputImpl.attachObject(this, object, index, renderWindow, vtk);
    }

    m_vtkInitialized = true;

    return vtk;
}

vtkRenderer* Viewer::myVtkObject(vtkUserData myUserData)
{
    auto vtk = MyVtkData::SafeDownCast(myUserData);

    if (!vtk) {
        qWarning() << "YIKES!! MyVtkData::SafeDownCast(myVtkData) FAILED";
        return {};
    }

    return vtk->renderer;
}


void Viewer::dispatch_async(std::function<void(vtkRenderWindow* renderWindow, vtkUserData renderData)> f, SharedData* d)
{    
    QQuickVtkItem::dispatch_async([
        pThis = QPointer<Viewer>(this),
        f = f,
        dispatchers = d->dispatchers()]
    (vtkRenderWindow* renderWindow, vtkUserData renderData)
    {
        if (!pThis) {
            qWarning() << "YIKES!!  I was deleted";
            return;
        }

       f(renderWindow, renderData);

       for(auto& p : dispatchers) {
           if (p) {
               auto viewer = qobject_cast<Viewer*>(p); if (viewer) {
                   if (pThis != viewer)
                       viewer->scheduleRender();
               } else {
                   QString s; QDebug(&s) << p;
                   qFatal("YIKES!! qobject_cast<Viewer*>(p) FAILED: %s", s.toStdString().c_str());
               }
           } else
               qWarning() << "YIKES!! dispatcher was deleted";
       }
    });
}

bool Viewer::event(QEvent* ev)
{
    // Forward camera changes
    switch (ev->type())
    {
    default:                          break;
    case QEvent::HoverEnter:          Q_FALLTHROUGH();
    case QEvent::HoverLeave:          Q_FALLTHROUGH();
    case QEvent::HoverMove:           Q_FALLTHROUGH();
    case QEvent::KeyPress:            Q_FALLTHROUGH();
    case QEvent::KeyRelease:          Q_FALLTHROUGH();
    case QEvent::FocusIn:             Q_FALLTHROUGH();
    case QEvent::FocusOut:            Q_FALLTHROUGH();
    case QEvent::MouseMove:           Q_FALLTHROUGH();
    case QEvent::MouseButtonPress:    Q_FALLTHROUGH();
    case QEvent::MouseButtonRelease:  Q_FALLTHROUGH();
    case QEvent::MouseButtonDblClick: Q_FALLTHROUGH();
    case QEvent::Wheel:               Q_FALLTHROUGH();
    case QEvent::TouchBegin:          Q_FALLTHROUGH();
    case QEvent::TouchUpdate:         Q_FALLTHROUGH();
    case QEvent::TouchEnd:            Q_FALLTHROUGH();
    case QEvent::TouchCancel:         
        break;
    }

    // Decode mouse events to click signal
    switch (ev->type())
    {
    case QEvent::MouseButtonPress: {
        auto e = static_cast<QMouseEvent*>(ev);
        m_click.button = e->button();
        m_click.localPos = e->localPos();
        break;
    }
    case QEvent::MouseButtonRelease: {
        auto e = static_cast<QMouseEvent*>(ev);
        if ((m_click.button == e->button()) && (m_click.localPos - e->localPos()).manhattanLength() < 3) {
            m_quickMouseEvent.reset(e->x(), e->y(), e->button(), e->buttons(), e->modifiers(), true, false, e->flags());
            emit clicked(&m_quickMouseEvent);
        }
        break;
    }
    default: 
        break;
    }

    return QQuickVtkItem::event(ev);
}

} }
