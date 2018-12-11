import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Controls.Universal 2.4
import QtQuick.Controls.Imagine 2.4

// C++ imports
import Minesweeper 1.0

ApplicationWindow {
    id: mainWindow
    visible: true

    // Disable geometry loading until all bugs are fixed
    /*onVisibleChanged: {
        if (visible) {
            visibility = settings.mainWindowVisibility
            width  = settings.mainWindowSize.width
            height = settings.mainWindowSize.height
        }
    }

    onVisibilityChanged: {
        if (visibility != ApplicationWindow.Hidden) {
            settings.mainWindowVisibility = visibility
            if (visibility != ApplicationWindow.Maximized) {
                width  = settings.mainWindowSize.width
                height = settings.mainWindowSize.height
            }
        }
    }

    onWidthChanged: {
        settings.mainWindowSize.width = width
    }
    onHeightChanged: {
        settings.mainWindowSize.height = height
    }
    */

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
}
