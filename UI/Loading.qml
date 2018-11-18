import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.4

Item {

    width: 720
    height: 500

    LoadingPopup {
        id: loading
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

        implicitWidth: 630
        implicitHeight: 380
    }

}
