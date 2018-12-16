import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.12

// C++ imports
import Minesweeper 1.0

Rectangle {
    id: root
    width: size
    height: size
    border.width: 1
    border.color: gridColor
    color: "transparent"

    property bool gameRunning: false
    property real size
    property Cell cell

    property int  row:               cell.row
    property int  column:            cell.column
    property bool activated:         cell.activated
    property bool flagged:           cell.flagged
    property bool flaggedUnknown:    cell.flaggedUnknown
    property int  content:           cell.content
    property int  value:             cell.value
    property bool exploted:          cell.exploted
    property bool highlightToReveal: cell.highlightToReveal
    property bool flash:             cell.flash
    property bool bothClick:         cell.bothClick
    property bool ripple:            cell.ripple

    property color gridColor: Qt.darker(Material.primary, 1.2)
    property color blockColor: Material.accent
    property color borderBlockColor: "#01000000"
    property real  borderFactorScaling: 0.1
    property real  borderWidth: size*borderFactorScaling > 3 ? 3 : size*borderFactorScaling
    property int   flashInterval: 300

    Timer {
        id: flashTimer
        interval: flashInterval
        onTriggered: cell.flash = false
    }

    onFlashChanged: {
        if (flash) {
            flashTimer.start()
        }
    }

    Rectangle {
        id: externalRectangle
        anchors.fill: parent

        width: size
        height: size
        border.width: borderWidth
        border.color: borderBlockColor
        color: highlightToReveal ? "#FFEB3B" : flash ? "#F44336" : activated ? "transparent" : blockColor

        Behavior on color { ColorAnimation { easing.type: Easing.InOutQuad } }

        ItemDelegate { // the cell button
            id: internalRectangle
            anchors.fill: parent
            anchors.margins: externalRectangle.border.width
            enabled: !activated

            onClicked: {
                cell.leftClick()
            }
        }

        Image {
            id: image
            anchors.fill: parent
            anchors.margins: externalRectangle.border.width + 2
            sourceSize: Qt.size(size, size)
            source: "qrc:/images/board/Board/" +
                    ((flagged && !activated) ? "flag" : (flaggedUnknown && !activated) ? "unknown" : (content === Cell.MINE && activated) ? "mine" : (content === Cell.VALUE && activated) ? "number" + value : "blank") +
                    ".svg"

            Image {
                id: crossImage
                anchors.fill: image
                sourceSize: Qt.size(size, size)
                source: "qrc:/images/board/Board/" +
                        ((flagged && content !== Cell.MINE && activated) ? "cross" : (flagged && content === Cell.MINE && activated) ? "check" : "blank") + ".svg"

            }

            Ripple {
                id: bangRipple
                width: parent.width
                height: parent.height
                pressed: false
                color: "#AAF44336"
                SequentialAnimation {
                    loops: Animation.Infinite
                    PropertyAction { target: bangRipple; property: "pressed"; value: true }
                    PauseAnimation { duration: 150 }
                    PropertyAction { target: bangRipple; property: "pressed"; value: false }
                    PauseAnimation { duration: 500 }
                    running: exploted
                    onStopped: {
                        bangRipple.pressed = false
                    }
                }
            }
        } // image


        // This is very important
        MouseArea {
            id: mouseArea
            anchors.fill: internalRectangle
            acceptedButtons: activated ? (Qt.LeftButton | Qt.RightButton | Qt.MidButton) : Qt.RightButton
            propagateComposedEvents: true
            preventStealing: true
            enabled: gameRunning

            property bool shining: false

            onClicked: {
                if (!activated && mouse.button === Qt.RightButton) {
                    cell.rightClick()
                }
            }

            onPressed: {
                if (activated && mouse.button === Qt.LeftButton && settings.oneClickRevealSiblings) {
                    if (clickRevealSiblingsRipple.pressed) {
                        clickRevealSiblingsRipple.pressed = false
                    }
                    clickRevealSiblingsRipple.pressed = true
                    cell.leftRightClick()
                    cell.bothClick = true
                    shining = true
                }

                if (mouse.buttons === Qt.LeftButton || mouse.buttons === Qt.RightButton) {
                    timerMidButton.start()
                }

                if ( (pressedButtons === Qt.MidButton) && containsMouse && activated ) {
                    if (clickRevealSiblingsRipple.pressed) {
                        clickRevealSiblingsRipple.pressed = false
                    }
                    clickRevealSiblingsRipple.pressed = true
                    cell.leftRightClick()
                    cell.bothClick = true
                    shining = true
                }
            }

            onReleased: {
                cell.leftRightUnclick()
                if ( (pressedButtons !== Qt.MidButton) && containsMouse && bothClick ) {
                    cell.tryToRevealSiblings()
                }
                cell.bothClick = false
                shining = false
                clickRevealSiblingsRippleAnimation.start()
            }

            onExited: {
                cell.leftRightUnclick()
                clickRevealSiblingsRippleAnimation.start()
            }

            onEntered: {
                if (shining) {
                    cell.leftRightClick()
                    clickRevealSiblingsRipple.pressed = true
                }
            }


            Ripple {
                id: clickRevealSiblingsRipple
                width: parent.width*3
                height: parent.height*3
                x: mouseArea.mouseX - width/2
                y: mouseArea.mouseY - height/2
                pressed: false
                color: Material.rippleColor

                SequentialAnimation {
                    id: clickRevealSiblingsRippleAnimation
                    PauseAnimation { duration: 200 }
                    PropertyAction { target: clickRevealSiblingsRipple; property: "pressed"; value: false }
                }
            }

            // Windows needs this timer to simulate mid button with both left and right clicks
            Timer {
                id: timerMidButton
                property int maxTries: 4 // 25*4 = 100 ms (at maximum) between a click and another
                property int triesLeft: maxTries
                interval: 25
                onTriggered: {
                    if (mouseArea.pressedButtons === (Qt.RightButton | Qt.LeftButton)) {
                        clickRevealSiblingsRipple.pressed = true
                        cell.leftRightClick()
                        cell.bothClick = true
                        mouseArea.shining = true
                        triesLeft = maxTries
                    } else {
                        if (triesLeft) {
                            triesLeft--
                            timerMidButton.start()
                        } else {
                            triesLeft = maxTries
                        }
                    }
                }
            }
        }
    } // external rectangle

}
