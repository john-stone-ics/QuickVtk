#include "quickVtkPolyDataMapper.h"
#include "quickVtkConeSource.h"
#include "quickVtkAlgorithm.h"
#include "quickVtkActor.h"
#include "quickVtkViewer.h"

#include "QtQmlTricksPlugin_SmartDataModels.h"

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QtQuick/QQuickWindow>

#include <QtGui/QGuiApplication>
#include <QtGui/QSurfaceFormat>

#include <QVTKRenderWindowAdapter.h>

#include "vtkLogger.h"
#include "vtkObject.h"


int main(int argc, char *argv[])
{
//    qputenv("QML_IMPORT_TRACE","1");

    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::OpenGL);
    QSurfaceFormat::setDefaultFormat(QVTKRenderWindowAdapter::defaultFormat());

#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterUncreatableType<quick::vtk::AbstractMapper3D >("Vtk", 1, 0, "AbstractMapper3D",  "!!");
    qmlRegisterType           <quick::vtk::Actor            >("Vtk", 1, 0, "Actor"                  );
    qmlRegisterType           <quick::vtk::ConeSource       >("Vtk", 1, 0, "ConeSource"             );
    qmlRegisterUncreatableType<quick::vtk::Mapper           >("Vtk", 1, 0, "Mapper",            "!!");
    qmlRegisterUncreatableType<quick::vtk::Object           >("Vtk", 1, 0, "Object",            "!!");
    qmlRegisterUncreatableType<quick::vtk::PolyDataAlgorithm>("Vtk", 1, 0, "PolyDataAlgorithm", "!!");
    qmlRegisterType           <quick::vtk::PolyDataMapper   >("Vtk", 1, 0, "PolyDataMapper"         );
    qmlRegisterUncreatableType<quick::vtk::Prop             >("Vtk", 1, 0, "Prop",              "!!");
    qmlRegisterUncreatableType<quick::vtk::Prop3D           >("Vtk", 1, 0, "Prop3D",            "!!");
    qmlRegisterUncreatableType<quick::vtk::Algorithm        >("Vtk", 1, 0, "Algorithm",         "!!");
    qmlRegisterUncreatableType<quick::vtk::AbstractMapper   >("Vtk", 1, 0, "AbstractMapper",    "!!");
    qmlRegisterType           <quick::vtk::Viewer           >("Vtk", 1, 0, "Viewer"                 );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
