import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Popup {
    id: loadingSplash

    visible: true
    closePolicy: Popup.NoAutoClose

    onOpened: {
        startupManager.emitReadySignal()
    }

    Item {
        id: itemCujaeInfo
        y: 8
        height: 170
        anchors.rightMargin: 8
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.left: parent.left
        opacity: startupManager.progress * 2 > 1.0 ? 1.0 : startupManager.progress * 2

        Image {
            id: imageCUJAE
            width: 96
            height: 96
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: imageCUJAEName.horizontalCenter
            anchors.bottom: imageCUJAEName.top
            anchors.bottomMargin: 16
            fillMode: Image.PreserveAspectFit
            source: "qrc:/splash/Splash/cujae.png"
            sourceSize: "96x96"
        }

//        Text {
//            id: imageCUJAEName
//            text: qsTr("text")
//        }
        Image {
            id: imageCUJAEName
            height: 64
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            fillMode: Image.PreserveAspectFit
            source: "qrc:/splash/Splash/cujaeName.png"
            sourceSize: "309x64"
        }

        Image {
            id: imageTiger
            width: 96
            height: 96
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: imageTigerName.horizontalCenter
            anchors.bottom: imageTigerName.top
            anchors.bottomMargin: 12
            fillMode: Image.PreserveAspectFit
            source: "qrc:/splash/Splash/tiger.png"
            sourceSize: "96x96"
        }

        Image {
            id: imageTigerName
            height: 42
            anchors.top: imageCUJAEName.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            fillMode: Image.PreserveAspectFit
            source: "qrc:/splash/Splash/tigerName.png"
            sourceSize: "235x42"
        }
    }

    Text {
        id: textAppName
        text: qsTr("MinesSeeker")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: itemCujaeInfo.bottom
        font.pointSize: 64
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: textAppVersion
        y: 263
        text: Qt.application.version
        font.pointSize: 16
        anchors.bottom: textAppName.bottom
        anchors.bottomMargin: 16
        anchors.left: textAppName.right
    }

    Text {
        id: textCopyright
        text: qsTr("Copyright © 2018 Carlos Enrique Pérez Sánchez.")
        anchors.topMargin: -16
        anchors.left: textAppName.left
        anchors.top: textAppName.bottom
        font.italic: true
        font.pointSize: 12
        opacity: startupManager.progress * 6 > 1.0 ? 1.0 : startupManager.progress * 6
    }

    ProgressBar {
        id: progressBar
        anchors.topMargin: 24
        anchors.top: textCopyright.bottom
        anchors.right: textAppName.right
        anchors.rightMargin: 0
        anchors.left: textAppName.left
        value: startupManager.progress
    }

    Text {
        id: textProgressMessage
        text: startupManager.progressMessage
        anchors.top: progressBar.bottom
        anchors.bottomMargin: 8
        anchors.right: progressBar.right
        anchors.left: progressBar.left
        font.pointSize: 9
    }
}
