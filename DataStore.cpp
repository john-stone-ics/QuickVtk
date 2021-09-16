#include "DataStore.h"
#include "VtkSource.h"

#include "vtkConeSource.h"
#include "vtkCylinderSource.h"
#include "vtkCubeSource.h"
#include "vtkSphereSource.h"

class DataStorePrivate 
{
public:
    DataStorePrivate(DataStore* q) : q_ptr(q) {}

    QQmlObjectListModel<VtkSourceBase>* vtkSources = nullptr;

private:
    Q_DISABLE_COPY(DataStorePrivate)
    Q_DECLARE_PUBLIC(DataStore)
    DataStore * const q_ptr;
};

DataStore::DataStore(QObject* parent) : QObject(parent), d_ptr(new DataStorePrivate(this))
{
    Q_D(DataStore);

    d->vtkSources = new std::remove_pointer_t<decltype(d->vtkSources)>;
    d->vtkSources->append(new VtkSource<vtkConeSource    >("Cone",     this));
    d->vtkSources->append(new VtkSource<vtkCylinderSource>("Cylinder", this));
    d->vtkSources->append(new VtkSource<vtkCubeSource    >("Cube",     this));
    d->vtkSources->append(new VtkSource<vtkSphereSource  >("Sphere",   this));
}

DataStore::~DataStore() = default;

QQmlObjectListModelBase* DataStore::vtkSources() const
{
    Q_D(const DataStore);
    
    return d->vtkSources;
}