#include <QApplication>
#include <Qt3DCore/QEntity>
#include <Qt3DWindow>
#include <Qt3DRender/QSceneLoader>
#include <Qt3DExtras/Qt3DExtras>
#include <Qt3DQuickExtras/Qt3DQuickExtras>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QGuiApplication>
#include <QUrl>
#include <QVector>
#include <QVBoxLayout>

class Scene : public Qt3DExtras::Qt3DWindow
{
public:
    Scene(QWidget * parent = nullptr) : Qt3DExtras::Qt3DWindow()
    {
    //    this->defaultFrameGraph()->setClearColor(Qt::gray);
        Qt3DCore::QEntity * sceneRoot = new Qt3DCore::QEntity;

        // Scene Camera
        Qt3DRender::QCamera *camera = this->camera();
        camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
        camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
        camera->setPosition(QVector3D(0.0f, 0.0f, 5.0f));
        camera->setNearPlane(0.001f);
        camera->setFarPlane(10000.0f);

        // For camera controls
        Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(sceneRoot);
        camController->setCamera(camera);

        Qt3DRender::QSceneLoader * sceneLoader = new Qt3DRender::QSceneLoader(sceneRoot);
        sceneLoader->setObjectName("monkey_face.dae");
        sceneRoot->addComponent(sceneLoader);
        sceneLoader->setSource(QUrl::fromLocalFile("C:\\Users\\tpgee\\Desktop\\monkey_face.dae"));
        this->setRootEntity(sceneRoot);
    }

    ~Scene() {
    }

private:
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Qt3DExtras::Quick::Qt3DQuickWindow view;
//    view.engine()->qmlEngine()->rootContext()->setContextProperty("_window", &view);
//    view.setSource(QUrl("../dae_import/main.qml"));
//    view.show();

    QWidget * ab = new QWidget;
    QVBoxLayout * lyt = new QVBoxLayout;
    QGraphicsScene * scene = new QGraphicsScene;
    Scene * sc = new Scene;
    scene->setBackgroundBrush(Qt::green);
    QWidget * w = QWidget::createWindowContainer(sc);
    w->setFixedSize(200,200);
    QGraphicsView * view = new QGraphicsView(scene);
    lyt->addWidget(view);
    lyt->addWidget(w);
    ab->setLayout(lyt);
    ab->show();

    return a.exec();
}
