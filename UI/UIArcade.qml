import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.4
import QtMultimedia 5.11

// C++ imports
import Minesweeper 1.0

// Here are dependences with C++ code: max rows and columns. update each as appropiate

Item {
    id: root

    property alias arcade: arcadeData
    property alias gameMode: boardFrontend.mode

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
            listModelMode.setProperty(i, "itemChecked", false)
        }

        listModelMode.setProperty(gameMode, "itemChecked", true)
        recordManager.setCurrentTable(gameMode)
    }

    focus: true

    UIBoard {
        id: boardFrontend

        anchors.top: rectangleMenu.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: drawer.dragMargin

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
        sequence: buttonNewGame.newGameShortcut
        onActivated: {
            if (buttonNewGame.enabled) {
                buttonNewGame.newGame()
            }
        }
    }

    // Pause the game with SPACE
    Keys.onSpacePressed: {
        if (roundButtonPause.enabled) {
            roundButtonPause.pause()
        }
    }

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
        ColorAnimation { target: remainingMinesGroup; property: "textColor"; to: "#F44336"; easing.type: Easing.InOutQuad } // Material.Red
        ColorAnimation { target: remainingMinesGroup; property: "textColor"; to: "black"; easing.type: Easing.InOutQuad; duration: 500 }
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

        /*  I can find a fair way to implement the statistics backend
         *  because I cant' know who user just lose, so I can't count
         *  the losed games for a particular user.
         */
        onWin: {
            lastGameCleared = true
//            recordManager.addGame(gameMode, true)
        }

        onLose: {
            if (soundMineHit.volume > 0.0) {
                soundMineHit.play()
            }
            lastGameCleared = false
//            recordManager.addGame(gameMode, false)
        }
    }


    property var delegateComponentMap: {
        "RadioDelegate": radioDelegateComponent,
                "ItemDelegate" : itemDelegateComponent
    }
    property var modeMap: { // useless right now
        "Beginner": Board.BEGINNER,
                "Medium":   Board.MEDIUM,
                "Expert":   Board.EXPERT,
                "Custom":   Board.CUSTOM
    }
    property var labelTextMap: { // useless right now
        "Play mode": playModeString,
                "Beginner":          beginnerModeString,
                "Medium":            mediumModeString,
                "Expert":            expertModeString,
                "Custom":            customModeString,
                "Statistics":        statisticsString,
                "Scores":            scoresString,
                "Settings":          settingsString,
                "Preferences":       preferencesString,
                "About":             aboutString + "...",
                "About MinesSeeker": aboutMinesSeekerString,
                "About Qt":          aboutQtString,
                "License":           licenseString
    }
    property string typeName: listView.section.property

    // for translation (if not the drawer strings are no translated with QQMLEngine::retranslate())
    property string playModeString:         qsTr("Play mode")
    property string beginnerModeString:     qsTr("Beginner")
    property string mediumModeString:       qsTr("Medium")
    property string expertModeString:       qsTr("Expert")
    property string customModeString:       qsTr("Custom")
    property string statisticsString:       qsTr("Statistics")
    property string scoresString:           qsTr("Scores")
    property string settingsString:         qsTr("Settings")
    property string preferencesString:      qsTr("Preferences")
    property string aboutString:            qsTr("About")
    property string aboutMinesSeekerString: aboutString + " " + Qt.application.name
    property string aboutQtString:          qsTr("About Qt")
    property string licenseString:          qsTr("License")

    // Side pane
    Drawer {
        id: drawer
        width: root.width/3 < 250 ? 250 : root.width/3
        height: root.height
        dragMargin: 3*Qt.styleHints.startDragDistance

        ListModel {
            id: listModelMode

            // Play mode
            ListElement {
                type: "RadioDelegate"
                typeText: "Play mode"
                mode: Board.BEGINNER
                text: "Beginner"
                itemChecked: true
            }
            ListElement {
                type: "RadioDelegate"
                typeText: "Play mode"
                mode: Board.MEDIUM
                text: "Medium"
                itemChecked: false
            }
            ListElement {
                type: "RadioDelegate"
                typeText: "Play mode"
                mode: Board.EXPERT
                text: "Expert"
                itemChecked: false
            }
            ListElement {
                type: "RadioDelegate"
                typeText: "Play mode"
                mode: Board.CUSTOM
                text: "Custom"
                itemChecked: false
            }


            // Statistics

            /*  I can find a fair way to implement the statistics backend
             *  because I cant' know who user just lose, so I can't count
             *  the losed games for a particular user.
             */
            /*ListElement {
                type: "ItemDelegate"
                typeText: "Statistics"
                mode: -1
                text: "Statistics"
                itemChecked: false
            }*/
            ListElement {
                type: "ItemDelegate"
                typeText: "Statistics"
                mode: -2
                text: "Scores"
                itemChecked: false
            }

            // Settings
            ListElement {
                type: "ItemDelegate"
                typeText: "Settings"
                mode: -10
                text: "Preferences"
                itemChecked: false
            }

            // About...
            ListElement {
                type: "ItemDelegate"
                typeText: "About"
                mode: -20
                text: "About MinesSeeker"
                itemChecked: false
            }
            ListElement {
                type: "ItemDelegate"
                typeText: "About"
                mode: -21
                text: "About Qt"
                itemChecked: false
            }
            ListElement {
                type: "ItemDelegate"
                typeText: "About"
                mode: -22
                text: "License"
                itemChecked: false
            }
        } // listModelMode

        // MODE
        ButtonGroup {
            id: radioButtonGroup
        }
        Component {
            id: radioDelegateComponent

            RadioDelegate {
                text: labelTextMap[labelText]
                ButtonGroup.group: radioButtonGroup
                checked: isChecked
                highlighted: isChecked
                Material.foreground: highlighted ? Material.accent : menuLanguage.materialTextColor

                property int mode: typeMode
                onClicked: {
                    if (mode !== gameMode || gameMode === Board.CUSTOM) {
                        listView.currentIndex = mode
                        if (mode === Board.CUSTOM) {
                            inputRowsColumnsDialog.open()
                        } else if (arcade.running) {
                            confirmModeChangeDialog.open()
                        } else {
                            arcade.changeMode(listView.currentIndex)
                            drawer.close()
                        }
                    }
                } // onClicked event
            } // RadioDelegate
        } // radioDelegateComponent

        Component {
            id: itemDelegateComponent

            ItemDelegate {
                text: labelTextMap[labelText]

                onClicked: {
                    if (typeMode == -1) {
                        statistics.open()
                    } else if (typeMode == -2) {
                        recordViewer.open()
                    } else if (typeMode == -10) {
                        preferencestDialog.open()
                    } else if (typeMode == -20) {
                        aboutDialog.open()
                    } else if (typeMode == -21) {
                        aboutQtDialog.open()
                    } else if (typeMode == -22) {
                        licenseDialog.open()
                    }
                } // onClicked event
            } // ItemDelegate
        } // itemDelegateComponent

        // VIEW
        ListView {
            id: listView

            focus: true
            currentIndex: 0
            anchors.fill: parent

            model: listModelMode
            section.property: "typeText"
            section.delegate: Pane {
                width: listView.width
                height: sectionLabel.implicitHeight + 20

                Label {
                    id: sectionLabel
                    text: labelTextMap[section]
                    font.pointSize: 24
                    anchors.centerIn: parent
                }
            }

            delegate: Loader {
                id: delegateLoader
                width: listView.width
                sourceComponent: delegateComponentMap[type]

                property string labelText: text
                property string typeMode: mode
                property bool isChecked: itemChecked
            }
        } // listView

    } // drawer


    // Menus, windows and Popups
    Menu {
        id: menuLanguage

        property color iconColor: "transparent"
        property color materialTextColor: Material.foreground

        MenuItem {
            text: "English"
            icon.source: "qrc:/languages/Languages/en.svg"
            icon.color: menuLanguage.iconColor
            Material.foreground: settings.language === "en" ? Material.accent : menuLanguage.materialTextColor
            onClicked: {
                settings.language = "en"
            }
        }
        MenuItem {
            text: "Español"
            icon.source: "qrc:/languages/Languages/es.svg"
            icon.color: menuLanguage.iconColor
            Material.foreground: settings.language === "es" ? Material.accent : menuLanguage.materialTextColor
            onClicked: {
                settings.language = "es"
            }
        }
    }

    SavingPopup {
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay
        visible: recordManager.busy
        modal: true
        onOpened: {
            if (inputRecordDialog.visible) {
                inputRecordDialog.close()
            }
            if (recordViewer.visible) {
                recordViewer.close()
            }
            if (drawer.visible) {
                drawer.close()
            }
        }
    }

    DialogInputRecord {
        id: inputRecordDialog
    }

    DialogPause {
        id: pauseDialog
    }

    Dialog {
        id: confirmModeChangeDialog

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

        focus: true
        modal: true
        title: qsTr("Confirm mode change")
        standardButtons: Dialog.Yes | Dialog.No

        Label {
            elide: Label.ElideRight
            text: qsTr("<b><i>Changing the mode will end the current game.</i></b><br>" +
                       "Continue?")
        }

        onAccepted: {
            arcade.changeMode(listView.currentIndex)
            drawer.close()
        }
        onRejected: {
            listModelMode.setProperty(gameMode, "itemChecked", false)
            listModelMode.setProperty(gameMode, "itemChecked", true)
            listView.currentIndex = gameMode
        }
    } // confirmModeChangeDialog

    Dialog {
        id: inputRowsColumnsDialog

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

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
            drawer.close()
        }
        onRejected: {
            listModelMode.setProperty(gameMode, "itemChecked", false)
            listModelMode.setProperty(gameMode, "itemChecked", true)
            listView.currentIndex = gameMode
            spinBoxRows.value = boardFrontend.board.customGameRows
            spinBoxColumns.value = boardFrontend.board.customGameColumns
            spinBoxMines.value = boardFrontend.board.customGameMines
        }
    } // inputRowsColumnsDialog

    // Statistics
    Statistics {
        id: statistics
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

        modal: true
    }

    RecordViewer {
        id: recordViewer
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

        modal: true
    }

    // Settings
    Preferences {
        id: preferencestDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

        implicitWidth: 500
        implicitHeight: 450
    }

    // About
    About {
        id: aboutDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

        source: "qrc:/images/appIcon.png"
        sourceSize: "128x128"
        simple: settings.firstTime
        changelogFirst: settings.firstTimeAfterUpgrade

        onClosed: {
            settings.firstTime = false
            settings.firstTimeAfterUpgrade = false
        }
    }
    AboutQt {
        id: aboutQtDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay

        source: "qrc:/images/QtProject-qtcreator.png"
        sourceSize: "128x128"
    }
    License {
        id: licenseDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        parent: Overlay.overlay
    }
}
