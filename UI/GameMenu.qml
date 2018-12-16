import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import Minesweeper 1.0

// Side pane
Drawer {
    id: drawer
    width: root.width/3 < 250 ? 250 : root.width/3
    height: root.height
    dragMargin: 3*Qt.styleHints.startDragDistance

    property alias gameMenuListModel: listModelMode
    property alias gameMenuListView: listView


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

    // for translation (if not the game menu strings are no translated with QQMLEngine::retranslate())
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


        // Statistics & Scores
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
