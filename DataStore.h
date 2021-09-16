#pragma once

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

#include "QQmlObjectListModel.h"

class DataStorePrivate;
class DataStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlObjectListModelBase* vtkSources READ vtkSources CONSTANT)

public:
    DataStore(QObject* parent=nullptr);
    ~DataStore() override;

    QQmlObjectListModelBase* vtkSources() const;

private:
    Q_DISABLE_COPY(DataStore)
    Q_DECLARE_PRIVATE(DataStore)
    QScopedPointer<DataStorePrivate> const d_ptr;
};