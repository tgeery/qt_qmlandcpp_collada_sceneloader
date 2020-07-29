#include "mainwindow.h"

#include <QApplication>
#include <Qt3DCore/QEntity>
#include <Qt3DWindow>
#include <Qt3DRender/QSceneLoader>
#include <Qt3DExtras/Qt3DExtras>
#include <Qt3DQuickExtras/Qt3DQuickExtras>
#include <QGraphicsView>
#include <QGuiApplication>
#include <QUrl>
#include <QVector>

int main(int argc, char *argv[])
{
//    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
//    format.setRenderableType(QSurfaceFormat::OpenGLES);
//    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow w;

//    Qt3DExtras::Quick::Qt3DQuickWindow view;
//    view.engine()->qmlEngine()->rootContext()->setContextProperty("_window", &view);
//    view.setSource(QUrl("../dae_import/main.qml"));
//    view.show();

    Qt3DExtras::Qt3DWindow view;
//    view.defaultFrameGraph()->setClearColor(Qt::gray);
    Qt3DCore::QEntity * sceneRoot = new Qt3DCore::QEntity;

    // Scene Camera
    Qt3DRender::QCamera *camera = view.camera();
    camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    camera->setViewCenter(QVector3D(0.0f, 3.5f, 0.0f));
    camera->setPosition(QVector3D(0.0f, 3.5f, 25.0f));
    camera->setNearPlane(0.001f);
    camera->setFarPlane(10000.0f);

    // For camera controls
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(sceneRoot);
    camController->setCamera(camera);

    Qt3DCore::QEntity * sceneLoaderEntity = new Qt3DCore::QEntity(sceneRoot);
    Qt3DRender::QSceneLoader * sceneLoader = new Qt3DRender::QSceneLoader(sceneLoaderEntity);
    sceneLoader->setObjectName("monkey_face.dae");
    sceneLoaderEntity->addComponent(sceneLoader);
    sceneLoader->setSource(QUrl::fromLocalFile("C:\\Users\\tpgee\\Desktop\\monkey_face.dae"));
    view.setRootEntity(sceneRoot);
//    view.show();

    QGraphicsView * gview = new QGraphicsView;
    QGraphicsScene * scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::green);
    gview->setScene(scene);
//    gview->show();
    view.show();

//    w.show();
    return a.exec();
}
