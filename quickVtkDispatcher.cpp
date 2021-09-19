#include <quickVtkDispatcher.h>

#include <QtCore/QCoreApplication>

#include <algorithm>

namespace quick { namespace vtk {

void Dispatcher::aboutToBeDeleted()
{
    for(auto p : qAsConst(m_aboutToBeDeleted))
        p->aboutToBeDeleted();
}

#if 0
//
/* ========================================================================= */
//

StrongDispatcherPtr::StrongDispatcherPtr(WeakDispatcherPtr& wp) : d(new Data(wp))
{}
StrongDispatcherPtr::Data::Data(WeakDispatcherPtr& wp) : _wp(wp)
{
    _wp.d->_m.lock();

    if (1 == ++wp.d->_n)
        _wp.d->_t = QThread::currentThreadId();

}
StrongDispatcherPtr::Data::~Data()
{
    if (0 == --_wp.d->_n)
        _wp.d->_t = 0;

    _wp.d->_m.unlock();
}
quick::vtk::Dispatcher* StrongDispatcherPtr::operator->()
{
    return d->_wp.d->_p;
}
StrongDispatcherPtr::operator quick::vtk::Dispatcher *()
{
    return d->_wp.d->_p;
}

//
/* ========================================================================= */
//

WeakDispatcherPtr::WeakDispatcherPtr(quick::vtk::Dispatcher* p) : d(new Data(p))
{}

WeakDispatcherPtr::Data::Data(quick::vtk::Dispatcher* p)
{
    if (!p)
        return;

    _m.lock();
    _p = p;
    _p->m_aboutToBeDeleted.insert(this);
    _m.unlock();
}

void WeakDispatcherPtr::Data::aboutToBeDeleted()
{
    _m.lock();
    if (QThread::currentThreadId() == _t)
        qFatal("YIKES!! dispatcher aboutToBeDeleted() while a StrongDispatcherPtr still exists in the same thread");
    _p = nullptr;
    _m.unlock();
}

WeakDispatcherPtr::Data::~Data()
{
    if (_n)
        qFatal("YIKES!! %s -- destroying while locked", Q_FUNC_INFO);

    _m.lock();
    if (_p) {
        _p->m_aboutToBeDeleted.remove(this);
        _p = nullptr;
    }
    _m.unlock();
}

StrongDispatcherPtr WeakDispatcherPtr::lock(bool canFail)
{
    if (!d)
        qFatal("YIKES!! %s -- locking while uninitialized", Q_FUNC_INFO);

    if (!d->_p && !canFail)
        qFatal("YIKES!! %s -- locking a nullptr", Q_FUNC_INFO);

    return StrongDispatcherPtr(*this);
}
#endif

//
/* ========================================================================= */
//

Dispatcher* SharedData::dispatcher()
{
    if (!m_quickVtkParents.size())
    {
        auto dispatcher = dynamic_cast<Dispatcher*>(this); if (dispatcher)
        {
            return dispatcher;
        }
        else
        {
            QString s; QDebug(&s) << this;
            qFatal("YIKES!! I'm not a Dispatcher but I should be: %s", s.toStdString().c_str());
            return nullptr;
        }
    }
    else
    {
        for (auto i = m_quickVtkParents.begin(); i != m_quickVtkParents.end(); )
        {            
            if (*i)
            {
                auto sharedData = dynamic_cast<SharedData*>(i->data()); if (sharedData)
                {
                    return sharedData->dispatcher();
                }
                else
                {
                    QString s; QDebug(&s) << i->data();
                    qFatal("YIKES!! found a non-SharedData parent: %s", s.toStdString().c_str());
                }
            }
        }

        QString s; QDebug(&s) << this;
        qFatal("YIKES!! all my parents have been deleted: %s", s.toStdString().c_str());
        return nullptr;
    }
}

void SharedData::addVtkParent(QObject * qobj)
{
    m_quickVtkParents.push_back(qobj);
}

void SharedData::delVtkParent(QObject * qobj)
{
    if (auto i = std::find(m_quickVtkParents.begin(), m_quickVtkParents.end(), qobj); i != m_quickVtkParents.end())
        m_quickVtkParents.erase(i);
    else
        qWarning() << "YIKES!! m_quickVtkParents.remove(qobj) FAILED: " << qobj;
}
} }
