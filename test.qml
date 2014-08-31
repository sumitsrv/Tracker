import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    width: 800;
    height: 600;

    Rectangle {
        id: mainWindow
        anchors.fill: parent;
        visible: true

        color: white

        //    Image {
        //        id: bubble
        //        source: "./images/speech-bubble.svg"
        //        smooth: true
        //        property real centerX: mainWindow.width / 2
        //        property real centerY: mainWindow.height / 2
        //        property real bubbleCenter: bubble.width / 2
        //        x: centerX - bubbleCenter
        //        y: centerY - bubbleCenter
        //    }
    }
}
