import QtQuick 2.12
import QtQuick.Controls.Material.impl 2.12 // 'private' module

// Tested in Qt 5.11. This is from Qt source code. So it's is no garranty that this code remains compatible in future versions of Qt.
Item {
    property alias clipRadius:   ripple.clipRadius
    property alias pressed:      ripple.pressed
    property alias active:       ripple.active
    property alias color:        ripple.color

    Ripple { // set clip to true to keep the ripple inside his parent
        id: ripple
        width: parent.width
        height: parent.height
        pressed: true // true activates the ripple effect, false deactivates it
        active: false
        color: "#AA000000"
    }
}
