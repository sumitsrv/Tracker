import QtQuick 2.2
import QtQuick.Controls 1.1
import QtSensors 5.3
import QtMultimedia 5.0

ApplicationWindow {
    id : cameraUI

    width: 800
    height: 480

    color: "black"
    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage

        imageCapture {
            onImageCaptured: {
                photoPreview.source = preview
                stillControls.previewAvailable = true
                cameraUI.state = "PhotoPreview"
            }
        }

        videoRecorder {
             resolution: "640x480"
             frameRate: 15
        }
    }

    VideoOutput {
        id: viewfinder
        visible: true

        x: 0
        y: 0
        width: parent.width
        height: parent.height

        source: camera
        autoOrientation: true
    }

}
