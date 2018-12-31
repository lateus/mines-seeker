import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

// C++ imports
import Minesweeper 1.0

Item {
    id: root

    property alias arcade: arcadeData
    property alias gameMode: boardFrontend.mode
    property alias menu: gameMenu

    property bool lastGameCleared: false
    property string gameModeString: gameMode === Board.BEGINNER ? qsTr("Beginner") : gameMode === Board.MEDIUM ? qsTr("Medium") : gameMode === Board.EXPERT ? qsTr("Expert") : qsTr("Custom")

    property real menuHeight: 57

    property var gameModeColorMap: {
        0: "#03A9F4",
                1: "#8BC34A",
                2: "#FF5722",
                3: "#FFFFFF"
    }
    property var gameModeColorMapOpacity: {
        0: "#5503A9F4",
                1: "#558BC34A",
                2: "#55FF5722",
                3: "#55555555"
    }

    onGameModeChanged: {
        for (var i = Board.BEGINNER; i <= Board.CUSTOM; ++i) {
            gameMenu.gameMenuListModel.setProperty(i, "itemChecked", false)
        }

        gameMenu.gameMenuListModel.setProperty(gameMode, "itemChecked", true)
        recordManager.setCurrentTable(gameMode)
    }

    focus: true

    UIBoard {
        id: boardFrontend

        anchors.top: rectangleMenu.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: gameMenu.dragMargin

        board: arcadeData.board
        onUiReadyChanged: {
            if (visible && settings.firstTime || settings.firstTimeAfterUpgrade) {
                aboutDialog.open()
                soundWelcome.play()
            }
        }
    }

    Image {
        id: imageArrow
        anchors.left: parent.left
        anchors.leftMargin: arrowMenuHoverMouseArea.containsMouse ? -18 : -24
        anchors.verticalCenter: parent.verticalCenter

        source: "qrc:/images/arcade/Arcade/arrowMenuPanel.svg"
        sourceSize: "48x512"

        MouseArea {
            id: arrowMenuHoverMouseArea
            anchors.fill: imageArrow
            hoverEnabled: true
            acceptedButtons: Qt.NoButton
        }

        ToolTip {
            text: qsTr("Drag right to open menu")
            visible: arrowMenuHoverMouseArea.containsMouse
            x: arrowMenuHoverMouseArea.mouseX + 10
            y: arrowMenuHoverMouseArea.mouseY - 10
        }

        Behavior on anchors.leftMargin { NumberAnimation {} }
    }


    // New game with CTRL+N
    Shortcut {
        sequence: rectangleMenu.newGameButton.newGameShortcut
        onActivated: {
            if (rectangleMenu.newGameButton.enabled) {
                rectangleMenu.newGameButton.newGame()
            }
        }
    }

    // Pause the game with SPACE
    Keys.onSpacePressed: {
        if (rectangleMenu.pauseGameButton.enabled) {
            rectangleMenu.pauseGameButton.pause()
        }
    }

    GameToolbar {
        id: rectangleMenu
    }

    // SOUNDS:
    SoundEffect {
        id: soundMineHit
        property int random: ~~(Math.random() * 1000) & 0x07
        source: "qrc:/soundEffects/Effects/mineHit" + random + ".wav"
        volume: settings.volume
        onPlayingChanged: {
            if (!playing) {
                if (!soundLose.playing && volume > 0.0) {
                    soundLose.play()
                }
                soundMineHit.random = ~~(Math.random() * 1000) & 0x07
            }
        }
    }
    SoundEffect {
        id: soundWelcome
        source: "qrc:/soundEffects/intro.wav"
        volume: settings.volume
    }
    SoundEffect {
        id: soundLose
        source: "qrc:/soundEffects/Effects/lose.wav"
        volume: settings.volume
    }
    SoundEffect {
        id: soundWin
        source: "qrc:/soundEffects/Effects/win.wav"
        volume: settings.volume
    }
    SoundEffect {
        id: soundNewRecord
        source: "qrc:/soundEffects/Effects/newRecord.wav"
        volume: settings.volume
    }
    SoundEffect {
        id: soundHacked
        loops: SoundEffect.Infinite
        source: "qrc:/soundEffects/Effects/hacked.wav"
        volume: settings.volume
    }
    SoundEffect {
        id: soundBigUnlock
        source: "qrc:/soundEffects/Effects/unlockManyCells.wav"
        volume: settings.volume/2
    }

    SequentialAnimation {
        id: flaggingDeniedAnimation
        ColorAnimation { target: rectangleMenu.minesLeftGroup; property: "textColor"; to: "#F44336"; easing.type: Easing.InOutQuad } // Material.Red
        ColorAnimation { target: rectangleMenu.minesLeftGroup; property: "textColor"; to: "black"; easing.type: Easing.InOutQuad; duration: 500 }
    }


    // C++
    Arcade {
        id: arcadeData
        board.firstClickClear: settings.boardGeneration === GeneralSettings.FirstClickClear

        onBigUnlock: {
            if (soundBigUnlock.volume > 0.0) {
                soundBigUnlock.play()
            }
        }

        onFlaggingDenied: {
            flaggingDeniedAnimation.stop()
            flaggingDeniedAnimation.start()
        }

        onFinished: { // always called after onWin or onLose
                inputRecordDialog.open()
        }
        onWin: {
            lastGameCleared = true
        }

        onLose: {
            if (soundMineHit.volume > 0.0) {
                soundMineHit.play()
            }
            lastGameCleared = false
        }
    }

    GameMenu {
        id: gameMenu
    }


    // Menus, dialogs and Popups
    GameLanguage {
        id: menuLanguage
    }

    SavingPopup {
        anchors.centerIn: parent
        visible: recordManager.busy
        modal: true
        onOpened: {
            if (inputRecordDialog.visible) {
                inputRecordDialog.close()
            }
            if (recordViewer.visible) {
                recordViewer.close()
            }
            if (gameMenu.visible) {
                gameMenu.close()
            }
        }
    }

    DialogInputRecord {
        id: inputRecordDialog
    }

    DialogPause {
        id: pauseDialog

        onAboutToShow: {
            blurEffectPause.radius = 128
        }
        onAboutToHide: {
            blurEffectPause.radius = 0
        }
    }

    DialogConfirmMode {
        id: confirmModeChangeDialog
    }

    DialogCustomMode {
        id: inputRowsColumnsDialog
    }

    // Records
    RecordViewer {
        id: recordViewer
    }

    // Settings
    Preferences {
        id: preferencestDialog
    }

    // About
    About {
        id: aboutDialog
        simple: settings.firstTime
        changelogFirst: settings.firstTimeAfterUpgrade

        onClosed: {
            settings.firstTime = false
            settings.firstTimeAfterUpgrade = false
        }
    }
    AboutQt {
        id: aboutQtDialog
    }
    License {
        id: licenseDialog
    }
}
