import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.4

Dialog {
    id: dialogStatistics

    implicitWidth:  480
    implicitHeight: 480
    standardButtons: Dialog.Close | Dialog.Reset

    Dialog {
        id: dialogConfirmReset1
        x: parent ? (parent.width - width) / 2 : 0
        y: parent ? (parent.height - height) / 2 : 0
        parent: Overlay.overlay
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        title: qsTr("Confirm statistics full reset")

        Text {
            text: qsTr("Reset and clear <b>all</b> <u>statistics</u> and <u>scores</u>?")
        }

        onAccepted: {
            dialogConfirmReset2.open()
        }
    }
    Dialog {
        id: dialogConfirmReset2
        x: parent ? (parent.width - width) / 2 : 0
        y: parent ? (parent.height - height) / 2 : 0
        parent: Overlay.overlay
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        title: dialogConfirmReset1.title

        Text {
            text: qsTr("This operation cannot be undone. It's permanent.<br>" +
                       "<b><i>Reset anyway?</i></b>")
        }

        onAccepted: {
            recordManager.reset()
            dialogStatistics.standardButton(Dialog.Reset).enabled = false
        }
    }

    onOpened: {
        standardButton(Dialog.Reset).enabled = progressBarTotalGamesStats.value > 0
    }

    onReset: { // erase the database
        dialogConfirmReset1.open()
    }

    Text {
        id: textStat
        text: qsTr("Statistics")
        font.pointSize: 72
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
    }

    GroupBox {
        id: groupBoxGamesWin
        width: 200
        height: 259
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: textStat.bottom
        anchors.topMargin: 8
        title: qsTr("Games win")

        ColumnLayout {
            anchors.fill: parent

            Item {
                id: rectangleBeginnerStats
                implicitHeight: 49
                Layout.fillWidth: true

                Text {
                    id: textBeginner
                    text: qsTr("Beginner")
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    font.pointSize: 9
                }

                Text {
                    id: textBeginnerStat
                    text: recordManager.beginnerGamesWin + '/' + recordManager.totalBeginnerGames
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    font.pointSize: 9
                }

                ProgressBar {
                    id: progressBarBeginnerStat
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.top: textBeginner.bottom
                    anchors.topMargin: 8
                    value: recordManager.beginnerGamesWin/recordManager.totalBeginnerGames
                }
            }

            Item {
                id: rectangleMediumStats
                implicitHeight: 49
                Layout.fillWidth: true

                Text {
                    id: textMedium
                    text: qsTr("Medium")
                    anchors.left: parent.left
                    font.pointSize: 9
                    anchors.top: parent.top
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                }

                Text {
                    id: textMediumStat
                    text: recordManager.mediumGamesWin + '/' + recordManager.totalMediumGames
                    anchors.right: parent.right
                    font.pointSize: 9
                    anchors.rightMargin: 8
                    anchors.top: parent.top
                    anchors.topMargin: 8
                }

                ProgressBar {
                    id: progressBarMediumStat
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.top: textMedium.bottom
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                    value: recordManager.mediumGamesWin/recordManager.totalMediumGames
                }
            }

            Item {
                id: rectangleExpertStats
                implicitHeight: 49
                Layout.fillWidth: true

                Text {
                    id: textExpert
                    text: qsTr("Expert")
                    anchors.left: parent.left
                    font.pointSize: 9
                    anchors.top: parent.top
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                }

                Text {
                    id: textExpertStat
                    text: recordManager.expertGamesWin + '/' + recordManager.totalExpertGames
                    anchors.right: parent.right
                    font.pointSize: 9
                    anchors.rightMargin: 8
                    anchors.top: parent.top
                    anchors.topMargin: 8
                }

                ProgressBar {
                    id: progressBarExpertStat
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.rightMargin: 8
                    anchors.top: textExpert.bottom
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                    value: recordManager.expertGamesWin/recordManager.totalExpertGames
                }
            }

            Item {
                id: rectangleCustomStats
                implicitHeight: 49
                Layout.fillWidth: true
                Text {
                    id: textCustom
                    text: qsTr("Custom")
                    anchors.left: parent.left
                    font.pointSize: 9
                    anchors.top: parent.top
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                }

                Text {
                    id: textCustomStats
                    text: recordManager.customGamesWin + '/' + recordManager.totalCustomGames
                    anchors.right: parent.right
                    font.pointSize: 9
                    anchors.rightMargin: 8
                    anchors.top: parent.top
                    anchors.topMargin: 8
                }

                ProgressBar {
                    id: progressBarCustomStats
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.top: textCustom.bottom
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                    value: recordManager.customGamesWin/recordManager.totalCustomGames
                }
            }
        }


    }

    GroupBox {
        id: groupBoxResume
        width: 200
        height: 154
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.top: textStat.bottom
        anchors.topMargin: 8
        title: qsTr("Resume")
        ColumnLayout {
            anchors.fill: parent
            Item {
                id: rectangleOfficialGamesStats
                implicitHeight: 49
                Text {
                    id: textOfficialGames
                    text: qsTr("Official games win")
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                    font.pointSize: 9
                }

                Text {
                    id: textOfficialGamesStat
                    text: (recordManager.beginnerGamesWin + recordManager.mediumGamesWin + recordManager.expertGamesWin) + '/' + (recordManager.totalBeginnerGames + recordManager.totalMediumGames + recordManager.totalExpertGames)
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    font.pointSize: 9
                }

                ProgressBar {
                    id: progressBarOfficialGamesStats
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.rightMargin: 8
                    anchors.top: textOfficialGames.bottom
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                    value: (recordManager.beginnerGamesWin + recordManager.mediumGamesWin + recordManager.expertGamesWin)/(recordManager.totalBeginnerGames + recordManager.totalMediumGames + recordManager.totalExpertGames)
                }
                Layout.fillWidth: true
            }

            Item {
                id: rectangleTotalStats
                implicitHeight: 49
                Text {
                    id: textTotalGames
                    text: qsTr("Total games win")
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                    font.pointSize: 9
                }

                Text {
                    id: textTotalGamesStats
                    text: (recordManager.beginnerGamesWin + recordManager.mediumGamesWin + recordManager.expertGamesWin + recordManager.customGamesWin) + '/' + (recordManager.totalBeginnerGames + recordManager.totalMediumGames + recordManager.totalExpertGames + recordManager.totalCustomGames)
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    font.pointSize: 9
                }

                ProgressBar {
                    id: progressBarTotalGamesStats
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.top: textTotalGames.bottom
                    anchors.leftMargin: 8
                    anchors.topMargin: 8
                    value: (recordManager.beginnerGamesWin + recordManager.mediumGamesWin + recordManager.expertGamesWin + recordManager.customGamesWin)/(recordManager.totalBeginnerGames + recordManager.totalMediumGames + recordManager.totalExpertGames + recordManager.totalCustomGames)
                }
                Layout.fillWidth: true
            }
        }
    }

}
