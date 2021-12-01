#include "quickVtkActor.h"
#include "quickVtkAlgorithm.h"
#include "quickVtkBooleanOperationPolyDataFilter.h"
#include "quickVtkBoxWidget2.h"
#include "quickVtkCone.h"
#include "quickVtkConeSource.h"
#include "quickVtkCylinderSource.h"
#include "quickVtkPolyDataMapper.h"
#include "quickVtkSphereSource.h"
#include "quickVtkTriangleFilter.h"
#include "quickVtkViewer.h"

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

    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::OpenGLRhi);
    QSurfaceFormat::setDefaultFormat(QVTKRenderWindowAdapter::defaultFormat());

#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    qSetMessagePattern(
                "%{if-info}I%{endif}"
                "%{if-debug}D%{endif}"
                "%{if-warning}\033[31mW\033[0m%{endif}"
                "%{if-critical}\033[31;mC\033[0m%{endif}"
                "%{if-fatal}\033[31;mF\033[0m%{endif}"
                "\033[34m %{threadid}\033[0m "
                "%{if-info}%{message}%{endif}"
                "%{if-debug}%{message}%{endif}"
                "%{if-warning}\033[31m%{message}\033[0m%{endif}"
                "%{if-critical}\033[31;m%{message}\033[0m%{endif}"
                "%{if-fatal}\033[31;m%{message}\033[0m%{endif}"
                " \033[1m\033[37m%{file}(%{line}):\033[0m "
               );
    QGuiApplication app(argc, argv);

    app.setFont(QFont("Noto Sans"));

    qmlRegisterType           <quick::math::Vector3                      >("Math",1, 0, "Vector3"                       );
    qmlRegisterUncreatableType<quick::vtk::AbstractMapper                >("Vtk", 1, 0, "AbstractMapper",           "!!");
    qmlRegisterUncreatableType<quick::vtk::AbstractMapper3D              >("Vtk", 1, 0, "AbstractMapper3D",         "!!");
    qmlRegisterUncreatableType<quick::vtk::AbstractWidget                >("Vtk", 1, 0, "AbstractWidget",           "!!");
    qmlRegisterType           <quick::vtk::Actor                         >("Vtk", 1, 0, "Actor"                         );
    qmlRegisterUncreatableType<quick::vtk::Algorithm                     >("Vtk", 1, 0, "Algorithm",                "!!");
    qmlRegisterType           <quick::vtk::BooleanOperationPolyDataFilter>("Vtk", 1, 0, "BooleanOperationPolyDataFilter");
    qmlRegisterType           <quick::vtk::BoxWidget2                    >("Vtk", 1, 0, "BoxWidget2"                    );
    qmlRegisterType           <quick::vtk::Cone                          >("Vtk", 1, 0, "Cone"                          );
    qmlRegisterType           <quick::vtk::ConeSource                    >("Vtk", 1, 0, "ConeSource"                    );
    qmlRegisterType           <quick::vtk::CylinderSource                >("Vtk", 1, 0, "CylinderSource"                );
    qmlRegisterUncreatableType<quick::vtk::ImageAlgorithm                >("Vtk", 1, 0, "ImageAlgorithm",           "!!");
    qmlRegisterUncreatableType<quick::vtk::ImplicitFunction              >("Vtk", 1, 0, "ImplicitFunction",         "!!");
    qmlRegisterUncreatableType<quick::vtk::Mapper                        >("Vtk", 1, 0, "Mapper",                   "!!");
    qmlRegisterUncreatableType<quick::vtk::Object                        >("Vtk", 1, 0, "Object",                   "!!");
    qmlRegisterUncreatableType<quick::vtk::PolyDataAlgorithm             >("Vtk", 1, 0, "PolyDataAlgorithm",        "!!");
    qmlRegisterType           <quick::vtk::PolyDataMapper                >("Vtk", 1, 0, "PolyDataMapper"                );
    qmlRegisterUncreatableType<quick::vtk::Prop                          >("Vtk", 1, 0, "Prop",                     "!!");
    qmlRegisterUncreatableType<quick::vtk::Prop3D                        >("Vtk", 1, 0, "Prop3D",                   "!!");
    qmlRegisterUncreatableType<quick::vtk::Property                      >("Vtk", 1, 0, "Property",                 "!!");
    qmlRegisterType           <quick::vtk::SphereSource                  >("Vtk", 1, 0, "SphereSource"                  );
    qmlRegisterUncreatableType<quick::vtk::Texture                       >("Vtk", 1, 0, "Texture",                  "!!");
    qmlRegisterType           <quick::vtk::TriangleFilter                >("Vtk", 1, 0, "TriangleFilter"                );
    qmlRegisterType           <quick::vtk::Viewer                        >("Vtk", 1, 0, "Viewer"                        );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    qDebug() << "app.exec()";
    return app.exec();
}
