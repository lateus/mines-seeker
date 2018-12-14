import QtQuick 2.11
import QtQuick.Controls 2.4

Dialog {
    id: dialogScores
    title: qsTr("Scores")
    implicitWidth: 650
    implicitHeight: 420
    standardButtons: Dialog.Close | Dialog.Reset
    Dialog {
        id: dialogConfirmReset1
        x: parent ? (parent.width - width) / 2 : 0
        y: parent ? (parent.height - height) / 2 : 0
        parent: Overlay.overlay

        implicitWidth: 400

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

        implicitWidth: 400

        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        title: dialogConfirmReset1.title

        Text {
            text: qsTr("This operation cannot be undone. It's permanent.<br>" +
                       "<b><i>Reset anyway?</i></b>")
        }

        onAccepted: {
            recordManager.reset()
            dialogScores.standardButton(Dialog.Reset).enabled = false
        }
    }

    onOpened: {
        standardButton(Dialog.Reset).enabled = (beginnerPage.time1 !== "00:00.000") || (mediumPage.time1 !== "00:00.000") || (expertPage.time1 !== "00:00.000") || (customPage.time1 !== "00:00.000")
        swipeViewRecords.currentIndex = recordManager.currentMode
    }

    onReset: { // erase the database
        dialogConfirmReset1.open()
    }

    TabBar {
        id: tabBarRecords
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        currentIndex: swipeViewRecords.currentIndex

        onCurrentIndexChanged: {
            if (currentIndex != swipeViewRecords.currentIndex) {
                swipeViewRecords.currentIndex = currentIndex
            }
        }

        TabButton {
            id: tabButtonBegginer

            text: qsTr("Beginner")

            onClicked: {
                swipeViewRecords.currentIndex = 0
            }
        }
        TabButton {
            id: tabButtonMedium

            text: qsTr("Medium")

            onClicked: {
                swipeViewRecords.currentIndex = 1
            }
        }
        TabButton {
            id: tabButtonExpert

            text: qsTr("Expert")

            onClicked: {
                swipeViewRecords.currentIndex = 2
            }
        }
        TabButton {
            id: tabButtonCustom

            text: qsTr("Custom")

            onClicked: {
                swipeViewRecords.currentIndex = 3
            }
        }
    } // tabBar

    SwipeView {
        id: swipeViewRecords
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: tabBarRecords.bottom
        anchors.bottomMargin: -24
        clip: true

        currentIndex: 0

        RecordViewerPage {
            id: beginnerPage

            color: "#03A9F4" // Material.Blue
            textColor: "white"

            name1:  recordManager.beginnerName1
            time1:  recordManager.beginnerTime1
            name2:  recordManager.beginnerName2
            time2:  recordManager.beginnerTime2
            name3:  recordManager.beginnerName3
            time3:  recordManager.beginnerTime3
            name4:  recordManager.beginnerName4
            time4:  recordManager.beginnerTime4
            name5:  recordManager.beginnerName5
            time5:  recordManager.beginnerTime5
            name6:  recordManager.beginnerName6
            time6:  recordManager.beginnerTime6
            name7:  recordManager.beginnerName7
            time7:  recordManager.beginnerTime7
            name8:  recordManager.beginnerName8
            time8:  recordManager.beginnerTime8
            name9:  recordManager.beginnerName9
            time9:  recordManager.beginnerTime9
            name10: recordManager.beginnerName10
            time10: recordManager.beginnerTime10
        }
        RecordViewerPage {
            id: mediumPage

            color: "#8BC34A" // Material.Green
            textColor: "white"

            name1:  recordManager.mediumName1
            time1:  recordManager.mediumTime1
            name2:  recordManager.mediumName2
            time2:  recordManager.mediumTime2
            name3:  recordManager.mediumName3
            time3:  recordManager.mediumTime3
            name4:  recordManager.mediumName4
            time4:  recordManager.mediumTime4
            name5:  recordManager.mediumName5
            time5:  recordManager.mediumTime5
            name6:  recordManager.mediumName6
            time6:  recordManager.mediumTime6
            name7:  recordManager.mediumName7
            time7:  recordManager.mediumTime7
            name8:  recordManager.mediumName8
            time8:  recordManager.mediumTime8
            name9:  recordManager.mediumName9
            time9:  recordManager.mediumTime9
            name10: recordManager.mediumName10
            time10: recordManager.mediumTime10
        }
        RecordViewerPage {
            id: expertPage

            color: "#FF5722" // Material.DeepOrange
            textColor: "white"

            name1:  recordManager.expertName1
            time1:  recordManager.expertTime1
            name2:  recordManager.expertName2
            time2:  recordManager.expertTime2
            name3:  recordManager.expertName3
            time3:  recordManager.expertTime3
            name4:  recordManager.expertName4
            time4:  recordManager.expertTime4
            name5:  recordManager.expertName5
            time5:  recordManager.expertTime5
            name6:  recordManager.expertName6
            time6:  recordManager.expertTime6
            name7:  recordManager.expertName7
            time7:  recordManager.expertTime7
            name8:  recordManager.expertName8
            time8:  recordManager.expertTime8
            name9:  recordManager.expertName9
            time9:  recordManager.expertTime9
            name10: recordManager.expertName10
            time10: recordManager.expertTime10
        }
        RecordViewerPage {
            id: customPage

            color: "#FFFFFF"

            name1:  recordManager.customName1
            time1:  recordManager.customTime1
            name2:  recordManager.customName2
            time2:  recordManager.customTime2
            name3:  recordManager.customName3
            time3:  recordManager.customTime3
            name4:  recordManager.customName4
            time4:  recordManager.customTime4
            name5:  recordManager.customName5
            time5:  recordManager.customTime5
            name6:  recordManager.customName6
            time6:  recordManager.customTime6
            name7:  recordManager.customName7
            time7:  recordManager.customTime7
            name8:  recordManager.customName8
            time8:  recordManager.customTime8
            name9:  recordManager.customName9
            time9:  recordManager.customTime9
            name10: recordManager.customName10
            time10: recordManager.customTime10
        }
    } // swipeViewRecords

    PageIndicator {
        id: indicator
        interactive: true

        count: swipeViewRecords.count
        currentIndex: swipeViewRecords.currentIndex

        onCurrentIndexChanged: {
            swipeViewRecords.currentIndex = currentIndex
        }

        anchors.bottom: swipeViewRecords.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
