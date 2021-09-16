#include <quickVtkDispatcher.h>

namespace quick { namespace vtk {

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

    _p = p;

    auto qobj = dynamic_cast<QObject*>(p); if (!qobj)
        qFatal("YIKES!! %s  --  dynamic_cast<QObject*>(%p) FAILED", Q_FUNC_INFO, p);

    _c = QObject::connect(qobj, &QObject::destroyed, [this] {
        _m.lock();
        _p = nullptr;
        _m.unlock();
    });
}
WeakDispatcherPtr::Data::~Data()
{
    if (_n)
        qFatal("YIKES!! %s -- destroying while locked", Q_FUNC_INFO);

    _m.lock();
    _p = nullptr;
    _m.unlock();
    QObject::disconnect(_c);
}

StrongDispatcherPtr WeakDispatcherPtr::lock()
{
    if (!d)
        qFatal("YIKES!! %s -- locking while uninitialized", Q_FUNC_INFO);

    return StrongDispatcherPtr(*this);
}

WeakDispatcherPtr::operator bool()
{
    return d && d->_p;
}

} }
