import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Item {
    width: 720
    height: 500

    LoadingSplash {
        id: loading
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

        implicitWidth: 630
        implicitHeight: 380
    }

}
