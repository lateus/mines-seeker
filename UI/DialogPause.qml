import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    id: pauseDialog

    anchors.centerIn: parent
    implicitWidth: 250

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
