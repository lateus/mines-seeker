import QtQuick 2.11
import QtQuick.Controls 2.4

Dialog {
    id: pauseDialog

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    parent: Overlay.overlay
    modal: true

    title: qsTr("PAUSED")

    Label {
        text: qsTr("Click anywhere to resume")

        MouseArea {
            anchors.fill: parent
            anchors.topMargin: -pauseDialog.header.height
            onClicked: {
                pauseDialog.close()
            }
        }
    }

    onClosed: {
        if (arcade.paused) {
            arcade.startTiming()
        }
        boardFrontend.visible = true
        rectangleMenu.visible = true
        imageArrow.visible = true
    }
} // pauseDialog
