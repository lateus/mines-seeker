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
            sourceSize: Qt.size(width, height)
        }

        Text {
            id: imageCUJAEName
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 82

            text: "cujae"
            font.family: "Humnst777"
            font.pointSize: 24
            font.bold: true
            color: "#086e54"
        }

        Image {
            id: imageTiger
            width: 106
            height: 106
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: imageTigerName.horizontalCenter
            anchors.bottom: imageTigerName.top
            anchors.bottomMargin: 12
            fillMode: Image.PreserveAspectFit
            source: "qrc:/splash/Splash/tiger.png"
            sourceSize: Qt.size(width, height)
        }

        Text {
            id: imageTigerName
            anchors.top: imageCUJAEName.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 28

            text: qsTr("Faculty of Engineering\nAutomation and Biomedicine")
            horizontalAlignment: Text.AlignHCenter
            font.family: "Humnst777"
            font.pointSize: 12
            font.bold: true
            color: "#333333"
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
