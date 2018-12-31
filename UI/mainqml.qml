import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.12

// C++ imports
import Minesweeper 1.0

ApplicationWindow {
    id: mainWindow
    visible: true

    minimumWidth: 460
    minimumHeight: 500
    title: qsTr("Mines Seeker")

    Material.accent: Material.Blue
    Material.primary: Material.BlueGrey

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "#DDDDDD"
        UIArcade {
            id: uiArcade
            anchors.fill: parent
        }
    }


    FastBlur {
        id: blurEffect
        visible: radius > 0
        anchors.fill: backgroundRectangle
        source: backgroundRectangle

//        cached: true
        Behavior on radius { NumberAnimation {} }
    }
}
