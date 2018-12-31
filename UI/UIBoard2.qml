import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

// C++ imports
import Minesweeper 1.0

Rectangle {
    id: root

    property Board board;

    property int  mode:            board.mode
    property int  rows:            board.uiRows
    property int  columns:         board.uiCols
    property int  mines:           board.mines
    property int  minesDiscovered: board.minesDiscovered
    property real progress:        board.progress
    property bool uiReady:         false

    Material.primary: "#DDDDDD"

    color: Material.primary

    Grid {
        id: grid
        anchors.centerIn: parent
        horizontalItemAlignment: Grid.AlignHCenter
        verticalItemAlignment: Grid.AlignVCenter
        rows: root.rows
        columns: root.columns
        spacing: -1

        Repeater {
            model: 1600
            onItemAdded: {
                if ((index + 1) % 10 === 0) {
                    startupManager.progress = (index + 1)/1600
                }
                if (index === 1599) {
                    uiReady = true
                }
            }

            delegate: UICell {
                property real widthHightRootReason: root.width/root.height
                property real rowColumnReason: columns/rows
                size: !uiReady ? 0 : widthHightRootReason < rowColumnReason ? (root.width)/columns : (root.height)/rows
                visible: uiReady && row >= 0 && column >= 0
                cell: board.itemAt(index)
                gameRunning: board.running
            }
        }
    }
}
