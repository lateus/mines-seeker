import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.3

Rectangle {
    id: rectangleMenu
    height: menuHeight
    color: "#ffffff"
    anchors.right: parent.right
    anchors.rightMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0
    anchors.top: parent.top
    anchors.topMargin: 0

    property alias newGameButton: buttonNewGame
    property alias pauseGameButton: roundButtonPause
    property alias minesLeftGroup: remainingMinesGroup

    Button {
        id: buttonNewGame
        text: qsTr("New game")
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        focusPolicy: Qt.NoFocus
        highlighted: enabled && !roundButtonPause.enabled

        property string newGameShortcut: qsTr("Ctrl+N")

        ToolTip.text: qsTr("Start a new game") + " (" + newGameShortcut + ")"
        ToolTip.visible: down || hovered

        enabled: (arcade.running && !arcade.paused) || arcade.waitingForOrder

        function newGame() {
            arcade.newGame()
        }

        onClicked: {
            newGame()
        }
    }

    Text {
        id: textElapsedTime
        text: arcade.stringTiming
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 24
        elide: Text.ElideNone
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "Source Code Pro"
        color: enabled ? "black" : "gray"
        enabled: arcade.running
    }

    RoundButton {
        id: roundButtonPause
        icon.source: "qrc:/images/arcade/Arcade/ic_pause_24px.svg"
        anchors.left: buttonNewGame.right
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        highlighted: true
        enabled: arcade.running && !arcade.paused

        property string pauseGameShortcut: qsTr("Space")

        ToolTip.text: qsTr("Pause the game") + " (" + pauseGameShortcut + ")"
        ToolTip.visible: down || hovered

        function pause() {
            arcade.pauseGame()
            boardFrontend.visible = false
            rectangleMenu.visible = false
            imageArrow.visible = false
            pauseDialog.open()
        }

        onClicked: {
            pause()
        }
    }

    ItemDelegate {
        id: buttonLanguage

        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8

        width: height

        contentItem: Image {
            anchors.fill: parent

            property real margin: 6
            anchors.margins: margin

            source: "qrc:/languages/Languages/" + settings.language + ".svg"
            sourceSize: Qt.size(menuHeight - margin, menuHeight - margin)
        }

        onClicked: {
            menuLanguage.popup()
        }
    }

    RowLayout {
        id: remainingMinesGroup
        property color textColor: "black"
        spacing: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.right: buttonLanguage.left
        anchors.rightMargin: 8
        Text {
            text: qsTr("Mines:")
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            color: remainingMinesGroup.textColor
        }
        Text {
            id: textMinesFoundReporter
            text: boardFrontend.minesDiscovered + '/' + boardFrontend.mines
            font.pointSize: 18
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            color: remainingMinesGroup.textColor
        }
    }
    ProgressBar {
        id: gameProgress
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -height/2
        Material.accent: Material.Pink

        from: 0.0
        to: 1.0
        value: boardFrontend.progress

        Behavior on value { NumberAnimation { easing.type: Easing.InOutQuad  } }
    }

} // rectangleMenu
