#include "quickVtkPolyDataMapper.h"
#include "quickVtkConeSource.h"
#include "quickVtkCylinderSource.h"
#include "quickVtkAlgorithm.h"
#include "quickVtkActor.h"
#include "quickVtkCone.h"
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

    qSetMessagePattern("\033[35m%{threadid}\033[0m  \033[1m%{file}(%{line}): \033[34m%{function}\033[0m -- %{message}");
    QGuiApplication app(argc, argv);

    app.setFont(QFont("Noto Sans"));

    qmlRegisterType           <quick::math::Vector3>         ("Math",1, 0, "Vector3"                );
    qmlRegisterUncreatableType<quick::vtk::AbstractMapper   >("Vtk", 1, 0, "AbstractMapper",    "!!");
    qmlRegisterUncreatableType<quick::vtk::AbstractMapper3D >("Vtk", 1, 0, "AbstractMapper3D",  "!!");
    qmlRegisterType           <quick::vtk::Actor            >("Vtk", 1, 0, "Actor"                  );
    qmlRegisterUncreatableType<quick::vtk::Algorithm        >("Vtk", 1, 0, "Algorithm",         "!!");
    qmlRegisterType           <quick::vtk::Cone             >("Vtk", 1, 0, "Cone"                   );
    qmlRegisterType           <quick::vtk::ConeSource       >("Vtk", 1, 0, "ConeSource"             );
    qmlRegisterType           <quick::vtk::CylinderSource   >("Vtk", 1, 0, "CylinderSource"         );
    qmlRegisterUncreatableType<quick::vtk::ImplicitFunction >("Vtk", 1, 0, "ImplicitFunction",  "!!");
    qmlRegisterUncreatableType<quick::vtk::Mapper           >("Vtk", 1, 0, "Mapper",            "!!");
    qmlRegisterUncreatableType<quick::vtk::Object           >("Vtk", 1, 0, "Object",            "!!");
    qmlRegisterUncreatableType<quick::vtk::PolyDataAlgorithm>("Vtk", 1, 0, "PolyDataAlgorithm", "!!");
    qmlRegisterType           <quick::vtk::PolyDataMapper   >("Vtk", 1, 0, "PolyDataMapper"         );
    qmlRegisterUncreatableType<quick::vtk::Prop             >("Vtk", 1, 0, "Prop",              "!!");
    qmlRegisterUncreatableType<quick::vtk::Prop3D           >("Vtk", 1, 0, "Prop3D",            "!!");
    qmlRegisterType           <quick::vtk::Viewer           >("Vtk", 1, 0, "Viewer"                 );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    qDebug() << "app.exec()";
    return app.exec();
}
