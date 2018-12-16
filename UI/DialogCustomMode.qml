import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Minesweeper 1.0


Dialog {
    id: inputRowsColumnsDialog

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    parent: Overlay.overlay

    implicitWidth: 400

    focus: true
    modal: true
    title: qsTr("Custom mode")
    standardButtons: arcade.running ? (Dialog.Yes | Dialog.No) : (Dialog.Ok | Dialog.Cancel)

    property alias rows:    spinBoxRows.value
    property alias columns: spinBoxColumns.value
    property alias mines:   spinBoxMines.value

    ColumnLayout {
        spacing: 20
        anchors.fill: parent
        Label {
            elide: Label.ElideRight
            text: qsTr("Please enter the rows, columns and mines:")
            Layout.fillWidth: true
        }
        ColumnLayout {
            spacing: 24
            Column {
                Text {
                    text: qsTr("Rows")
                }

                SpinBox {
                    id: spinBoxRows
                    focus: true
                    Layout.fillWidth: true

                    from: 5
                    to: 50 // must match the C++ value
                    value: boardFrontend.board.customGameRows
                }
            }
            Column {
                Text {
                    text: qsTr("Columns")
                }
                SpinBox {
                    id: spinBoxColumns
                    focus: true
                    Layout.fillWidth: true

                    from: 5
                    to: 50 // must match the C++ value
                    value: boardFrontend.board.customGameColumns
                }
            }

            Column {
                Text {
                    text: qsTr("Mines")
                }
                SpinBox {
                    id: spinBoxMines
                    focus: true
                    Layout.fillWidth: true

                    from: 1
                    to: spinBoxRows.value * spinBoxColumns.value - 10
                    value: boardFrontend.board.customGameMines
                }
            }
        }

        Label {
            elide: Label.ElideRight
            text: qsTr("<b><i>Changing the mode will end the current game.</i></b><br>" +
                       "Continue?")
            visible: arcade.running
            Layout.fillWidth: true
        }
    }

    onAccepted: {
        arcade.changeMode(Board.CUSTOM, spinBoxRows.value, spinBoxColumns.value, spinBoxMines.value)
        gameMenu.close()
    }
    onRejected: {
        gameMenu.gameMenuListModel.setProperty(gameMode, "itemChecked", false)
        gameMenu.gameMenuListModel.setProperty(gameMode, "itemChecked", true)
        gameMenu.gameMenuListView.currentIndex = gameMode
        spinBoxRows.value = boardFrontend.board.customGameRows
        spinBoxColumns.value = boardFrontend.board.customGameColumns
        spinBoxMines.value = boardFrontend.board.customGameMines
    }
} // inputRowsColumnsDialog
