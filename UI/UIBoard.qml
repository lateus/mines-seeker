import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

// C++ imports
import Minesweeper 1.0

// Here are dependences with C++ code: max rows and columns. update each as appropiate

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

    property real widthHightRootReason: width/height
    property real rowColumnReason: columns/rows

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
            model: 480
            onItemAdded: {
                if ((index + 1) % 10 === 0) {
                    startupManager.progress = (index + 1)/480
                }
                if (index === 479) {
                    uiReady = true
                }
            }

            delegate: UICell {
                size: !uiReady ? 0 : root.widthHightRootReason < root.rowColumnReason ? (root.width)/columns : (root.height)/rows
                visible: uiReady && row >= 0 && column >= 0
                cell: board.itemAt(index)
                gameRunning: board.running
            }
        }
    }
}
