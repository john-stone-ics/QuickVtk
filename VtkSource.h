#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

#include <vtkSmartPointer.h>

class vtkAlgorithm;

class VtkSourceBase: public QObject
{
    Q_OBJECT
public:
    VtkSourceBase(vtkAlgorithm* algorithm, QString name, QObject* parent=nullptr);
    ~VtkSourceBase() override;

    vtkSmartPointer<vtkAlgorithm> algorithm;
};

template<typename T>
class VtkSource : public VtkSourceBase
{
public:
    VtkSource(QString name, QObject* parent=nullptr) : VtkSourceBase(T::New(), name, parent) 
    {}

    T const* source() const { return static_cast<T>(algorithm); }
    T      * source()       { return static_cast<T>(algorithm); }
};
