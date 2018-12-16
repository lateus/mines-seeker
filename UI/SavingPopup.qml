import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Popup {
    id: loadingPopup
    closePolicy: Popup.NoAutoClose

    RowLayout {
        BusyIndicator {
            id: busyIndicator
            property color color: "#2196F3"
            Material.accent: color
            SequentialAnimation on color {
                loops: Animation.Infinite
                running: busyIndicator.visible
                ColorAnimation { from: "#2196F3"; to: "#00BCD4"; duration: 500 }
                ColorAnimation { from: "#00BCD4"; to: "#009688"; duration: 500 }
                ColorAnimation { from: "#009688"; to: "#4CAF50"; duration: 500 }
                ColorAnimation { from: "#4CAF50"; to: "#8BC34A"; duration: 500 }
                ColorAnimation { from: "#8BC34A"; to: "#CDDC39"; duration: 500 }
                ColorAnimation { from: "#CDDC39"; to: "#FFC107"; duration: 500 }
                ColorAnimation { from: "#FFC107"; to: "#FF9800"; duration: 500 }
                ColorAnimation { from: "#FF9800"; to: "#FF5722"; duration: 500 }
                ColorAnimation { from: "#FF5722"; to: "#F44336"; duration: 500 }
                ColorAnimation { from: "#F44336"; to: "#E91E63"; duration: 500 }
                ColorAnimation { from: "#E91E63"; to: "#9C27B0"; duration: 500 }
                ColorAnimation { from: "#9C27B0"; to: "#673AB7"; duration: 500 }
                ColorAnimation { from: "#673AB7"; to: "#3F51B5"; duration: 500 }
                ColorAnimation { from: "#3F51B5"; to: "#2196F3"; duration: 500 }
            }
        }
        Text {
            text: qsTr("Saving...")
            font.pointSize: 12
            Layout.fillWidth: true
        }
    }
}
