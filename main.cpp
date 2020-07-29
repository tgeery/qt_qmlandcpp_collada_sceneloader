#include "mainwindow.h"

#include <QApplication>
#include <Qt3DCore/QEntity>
#include <Qt3DWindow>
#include <Qt3DRender/QSceneLoader>
#include <Qt3DExtras/Qt3DExtras>
#include <Qt3DQuickExtras/Qt3DQuickExtras>
#include <QGuiApplication>
#include <QUrl>
#include <QVector>

class SceneWalker : public QObject
{
public:
    SceneWalker(Qt3DRender::QSceneLoader *loader) : m_loader(loader) { }

    void onStatusChanged();

private:
    void walkEntity(Qt3DCore::QEntity *e, int depth = 0);

    Qt3DRender::QSceneLoader *m_loader;
};

void SceneWalker::onStatusChanged()
{
    qDebug() << "Status changed:" << m_loader->status();
    if (m_loader->status() != Qt3DRender::QSceneLoader::Ready)
        return;

    // The QSceneLoader instance is a component of an entity. The loaded scene
    // tree is added under this entity.
    QVector<Qt3DCore::QEntity *> entities = m_loader->entities();

    // Technically there could be multiple entities referencing the scene loader
    // but sharing is discouraged, and in our case there will be one anyhow.
    if (entities.isEmpty())
        return;
    Qt3DCore::QEntity *root = entities[0];
    // Print the tree.
    walkEntity(root);

    // To access a given node (like a named mesh in the scene), use QObject::findChild().
    // The scene structure and names always depend on the asset.
    Qt3DCore::QEntity *e = root->findChild<Qt3DCore::QEntity *>(QStringLiteral("PlanePropeller_mesh")); // toyplane.obj
    if (e)
        qDebug() << "Found propeller node" << e << "with components" << e->components();
}

void SceneWalker::walkEntity(Qt3DCore::QEntity *e, int depth)
{
    Qt3DCore::QNodeVector nodes = e->childNodes();
    for (int i = 0; i < nodes.count(); ++i) {
        Qt3DCore::QNode *node = nodes[i];
        Qt3DCore::QEntity *entity = qobject_cast<Qt3DCore::QEntity *>(node);
        if (entity) {
            QString indent;
            indent.fill(' ', depth * 2);
            qDebug().noquote() << indent << "Entity:" << entity << "Components:" << entity->components();
            walkEntity(entity, depth + 1);
        }
    }
}

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
    view.defaultFrameGraph()->setClearColor(Qt::gray);
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
    SceneWalker sceneWalker(sceneLoader);
    QObject::connect(sceneLoader, &Qt3DRender::QSceneLoader::statusChanged, &sceneWalker, &SceneWalker::onStatusChanged);
    view.setRootEntity(sceneRoot);
    view.show();

//    w.show();
    return a.exec();
}
