import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Extras 2.12
import QtQuick 2.0

Entity {
    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                id: renderer
                camera: mainCamera
            }
        },
        SceneLoader {
            source: "file:///Users/tpgee/Desktop/monkey_face.dae"
        }
    ]
}
