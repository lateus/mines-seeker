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
        id: blurEffectPause
        visible: radius > 0
        anchors.fill: backgroundRectangle
        source: backgroundRectangle

        Behavior on radius { NumberAnimation {} }
    }
    FastBlur {
        id: blurEffectMenu
        visible: radius > 0
        anchors.fill: backgroundRectangle
        source: backgroundRectangle
        radius: uiArcade.menu.position * 128

        Behavior on radius { NumberAnimation {} }
    }
}
