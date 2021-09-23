#pragma once

#include <vtkSmartPointer.h>

#include <QtCore/QPointer>
#include <QtCore/QDebug>

class vtkRenderWindow;

template<typename QT, typename QV, typename VT, typename VV, typename VR, typename CR = QV>
void QProperty_setter_impl(QV v, bool force, QT* pThis, QV QT::*m_v, void (QT::*changed)(QV), VR (VT::*vtkSet)(VV), CR (*convert)(QV) = [](QV v){return v;}, void (*extra)(VT*,QT*) = [](VT*,QT*){})
{
    if (pThis->*m_v != v || force)
    {
        if (pThis->*m_v != v)
            emit (pThis->*changed)(pThis->*m_v = v);

        if (!pThis->m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

        pThis->dispatcher()->dispatch_async([
            pThis   = QPointer<QT>(pThis),
            m_v     = pThis->*m_v,
            vtkSet  = vtkSet,
            convert = convert,
            extra   = extra]
        (vtkRenderWindow*, typename QT::vtkUserData renderData)
        {
            if (!pThis) {
                qWarning() << "YIKES!! I was deleted";
                return;
            }

            auto vtkObj = pThis->myVtkObject(pThis->dispatcher()->lookup(pThis, renderData));

            if (vtkObj)
                return;

            (vtkObj->*vtkSet)(convert(m_v));

            extra(vtkObj, pThis);

        }, pThis);
    }
}

template<typename QT, typename QV, typename VT, typename VV, typename VR, typename CR = QV>
void QProperty_setter_impl(QV v, bool force, QT* pThis, QV QT::*m_v, void (QT::*changed)(QV), VR (VT::*vtkSet)(VV), void (*extra)(VT*,QT*))
{
    QProperty_setter_impl<QT,QV,VT,VV,VR,CR>(v, force, pThis, m_v, changed, vtkSet, [](QV v){return v;}, extra);
}


/*-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-= */



template<typename QT, typename QV, typename VT, typename VV, typename VR, typename CR = QV>
void QProperty_setter_impl2(QV v, bool force, QT* pThis, QV QT::*m_v, void (QT::*changed)(QV), VR (VT::*vtkSet)(VV), CR (*convert)(QV) = [](QV v){return v;}, void (*extra)(VT*,QT*) = [](VT*,QT*){})
{
    if (pThis->*m_v != v || force)
    {
        if (pThis->*m_v != v)
            emit (pThis->*changed)(pThis->*m_v = v);

        if (!pThis->m_vtkInitialized) {
            if (force)
                qFatal("!m_vtkInitialized && force==true");
            return;
        }

        pThis->dispatcher()->dispatch_async([
            pThis   = QPointer<QT>(pThis),
            vtkObj  = vtkSmartPointer<VT>(pThis->myVtkObject()),
            m_v     = pThis->*m_v,
            vtkSet  = vtkSet,
            convert = convert,
            extra   = extra]
        (vtkRenderWindow*, typename QT::vtkUserData renderData)
        {
            if (!pThis) {
                qWarning() << "YIKES!! I was deleted";
                return;
            }

            (vtkObj->*vtkSet)(convert(m_v));

            extra(vtkObj, pThis);

        }, pThis);
    }
}

template<typename QT, typename QV, typename VT, typename VV, typename VR, typename CR = QV>
void QProperty_setter_impl2(QV v, bool force, QT* pThis, QV QT::*m_v, void (QT::*changed)(QV), VR (VT::*vtkSet)(VV), void (*extra)(VT*,QT*))
{
    QProperty_setter_impl2<QT,QV,VT,VV,VR,CR>(v, force, pThis, m_v, changed, vtkSet, [](QV v){return v;}, extra);
}
