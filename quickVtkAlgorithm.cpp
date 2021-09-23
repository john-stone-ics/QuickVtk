#include "quickVtkAlgorithm.h"

#include <vtkAlgorithm.h>

namespace quick { namespace vtk {

Algorithm::Algorithm(QObject* parent) : Object(Object::Type::Algorithm, parent)
{
}

namespace {
void attachToObject(vtkSmartPointer<vtkAlgorithm> parent, vtkSmartPointer<vtkAlgorithm> child, int index)
{
    parent->SetInputConnection(index, child->GetOutputPort());
}

void detachFromObject(vtkSmartPointer<vtkAlgorithm> parent, vtkSmartPointer<vtkAlgorithm> child, int index)
{
    //TODO: figure out how to verify the existing connection here

    parent->RemoveInputConnection(index, child->GetOutputPort());
}

static void append(QQmlListProperty<Algorithm>* l, Algorithm* object)
{
    if (object->isVolatile()) {
        qWarning() << "YIKES!! attempted to attach a volatile object to this non-volatile algorithm";
        return;
    }

    auto pThis = qobject_cast<Algorithm*>(l->object);
    auto* list = reinterpret_cast<QList<Algorithm*>*>(l->data);

    list->append(object);
    object->addVtkParent(pThis);

    attachToObject(pThis->m_vtkAlgorithm, object->m_vtkAlgorithm, list->count());
}
static int count(QQmlListProperty<Algorithm>* l)
{
    return reinterpret_cast<QList<Algorithm*>*>(l->data)->count();
}
static Algorithm* at(QQmlListProperty<Algorithm>* l, int i)
{
    return reinterpret_cast<QList<Algorithm*>*>(l->data)->at(i);
}
static void clear(QQmlListProperty<Algorithm>* l)
{
    auto pThis = qobject_cast<Algorithm*>(l->object);
    auto* list = reinterpret_cast<QList<Algorithm*>*>(l->data);

    for(int i=0; i<list->count(); ++i) {
        list->at(i)->delVtkParent(pThis);
        detachFromObject(pThis->m_vtkAlgorithm, list->at(i)->m_vtkAlgorithm, i);
    }

    return list->clear();
}
}

QQmlListProperty<Algorithm> Algorithm::input()
{
    return QQmlListProperty<Algorithm>(this, &m_input, append, count, at, clear);
}

Algorithm::vtkUserData Algorithm::initializeVTK(vtkRenderWindow* renderWindow, vtkUserData renderData)
{
    if (!m_vtkInitialized)
    {
        m_vtkAlgorithm = makeAlgorithm();
        m_vtkInitialized = true;
    }

    for (int i=0; i<m_input.count(); ++i)
        attachToObject(m_vtkAlgorithm, m_input.at(i)->m_vtkAlgorithm, i);

    return nullptr;
}

vtkAlgorithm* Algorithm::myVtkObject(vtkUserData myUserData) const
{
    if (myUserData)
        qWarning() << "YIKES!! is non-volitile but was called with non-null myUserData";

    return m_vtkAlgorithm;
}

bool Algorithm::isVolatile() const
{
    return false;
}

} }
