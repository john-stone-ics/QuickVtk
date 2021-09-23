#include <quickVtkDispatcher.h>

#include <QtCore/QCoreApplication>

#include <algorithm>

namespace quick { namespace vtk {

void Dispatcher::aboutToBeDeleted()
{
    for(auto p : qAsConst(m_aboutToBeDeleted))
        p->aboutToBeDeleted();
}


/*-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-= */


QSet<SharedData*> SharedData::s_instances;

SharedData::SharedData()
{
    s_instances.insert(this);
}
SharedData::~SharedData()
{
    s_instances.remove(this);
}

bool SharedData::findDispatchers(std::function<bool(Dispatcher*)> f)
{
    if (!m_quickVtkParents.size())
    {
        auto dispatcher = dynamic_cast<Dispatcher*>(this); if (dispatcher)
        {
            return f(dispatcher);
        }
        else
        {
            QString s; QDebug(&s) << this;
            qFatal("YIKES!! I'm not a Dispatcher but I should be: %s", s.toStdString().c_str());
        }
    }
    else
    {
        bool found = false; for(auto const& p : qAsConst(m_quickVtkParents))
        {
            if (p)
            {
                auto sharedData = dynamic_cast<SharedData*>(p.data()); if (sharedData)
                {
                    if (sharedData->findDispatchers(f))
                        return true;
                }
                else
                {
                    QString s; QDebug(&s) << p.data();
                    qFatal("YIKES!! found a non-SharedData parent: %s", s.toStdString().c_str());
                }
                found  = true;
            }
        }

        if (!found) {
            QString s; QDebug(&s) << this;
            qFatal("YIKES!! all my parents have been deleted: %s", s.toStdString().c_str());
        }
    }

    return false;
}

void SharedData::refreshCaches()
{
    m_dispatcher = nullptr;
    findDispatchers([this](Dispatcher* d) {
        m_dispatcher = d;
        return true;
    });

    m_dispatchers.clear();
    findDispatchers([this](Dispatcher *d) {
       auto qobj = dynamic_cast<QObject*>(d); if (qobj) {
           if (!m_dispatchers.contains(qobj))
               m_dispatchers.append(qobj);
       } else {
           QString s; QDebug(&s) << d;
           qFatal("YIKES!! this dispatcher is not a QObject but it should be: %s", s.toStdString().c_str());
       }
       return false;
    });

    m_dirtyCache = false;
}

Dispatcher* SharedData::dispatcher()
{
    if (m_dirtyCache)
        refreshCaches();

    return m_dispatcher;
}

QVector<QPointer<QObject>> SharedData::dispatchers()
{
    if (m_dirtyCache)
        refreshCaches();

    return m_dispatchers;
}

void SharedData::addVtkParent(QObject * qobj)
{
    m_quickVtkParents.push_back(qobj);

    for(auto pThis : qAsConst(s_instances))
        pThis->m_dirtyCache = true;
}

void SharedData::delVtkParent(QObject * qobj)
{
    if (auto i = std::find(m_quickVtkParents.begin(), m_quickVtkParents.end(), qobj); i != m_quickVtkParents.end())
        m_quickVtkParents.erase(i);
    else
        qWarning() << "YIKES!! m_quickVtkParents.remove(qobj) FAILED: " << qobj;

    for(auto pThis :qAsConst(s_instances))
        pThis->m_dirtyCache = true;
}
} }
