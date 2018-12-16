import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Dialog {
    id: inputRecordDialog

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    parent: Overlay.overlay
    clip: true

    implicitWidth:  lastGameCleared && newScore ? 410 : lastGameCleared ? 360 : 230
    implicitHeight: lastGameCleared && newScore ? 390 : lastGameCleared ? 290 : 130

    focus: true
    modal: true
    title: lastGameCleared ? gameModeString + " - " + qsTr("CONGRATULATIONS!") : ""
    standardButtons: lastGameCleared ? Dialog.Ok | Dialog.Cancel : Dialog.Retry | Dialog.Cancel
    closePolicy: Popup.CloseOnEscape

    property bool newScore: (arcade.recordMs < recordManager.maxRecordMs) && arcade.recordMs


    Image {
        id: badge
        anchors.top: parent.top
        anchors.topMargin: -60
        anchors.left: parent.left
        anchors.leftMargin: -100
        source: "qrc:/images/arcade/Arcade/medal0.svg"
        sourceSize: "256x256"

        visible: lastGameCleared && inputRecordDialog.newScore
    }

    ColumnLayout {
        spacing: 20
        anchors.fill: parent
        anchors.bottomMargin: -32
        Text {
            id: newRecordText
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: lastGameCleared && inputRecordDialog.newScore
            font.pointSize: 32
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: qsTr("<b>NEW RECORD!</b><br>") + arcade.recordString

            SequentialAnimation on color {
                loops: Animation.Infinite
                PropertyAction { target: ripple; property: "pressed"; value: true }
                ColorAnimation { from: "black"; to: gameModeColorMap[gameMode]; duration: 250 }
                PropertyAction { target: ripple; property: "pressed"; value: false }
                ColorAnimation { from: gameModeColorMap[gameMode]; to: "black"; duration: 300 }
                running: newRecordText.visible
            }

            Ripple {
                id: ripple
                property real rippleSizeScalingFactor: 0.75
                anchors.centerIn: parent
                width: parent.width * rippleSizeScalingFactor
                height: parent.height * rippleSizeScalingFactor
                color: gameModeColorMapOpacity[gameMode]
            }
        }

        RowLayout {
            spacing: 8
            visible: lastGameCleared && !inputRecordDialog.newScore
            Text {
                id: textOwnRecord
                Layout.fillWidth: true
                text: qsTr("<b>Your score: ") + arcade.recordString + "</b>"
            }
            Text {
                id: maxRecord
                horizontalAlignment: Text.AlignRight
                text: qsTr("Record: ") + recordManager.maxRecordString
            }
        }

        Label {
            id: labelInputNameOrLost
            text: lastGameCleared ? qsTr("Enter your name:") : qsTr("<b>You have lost</b>")
            font.pointSize: lastGameCleared ? 10 : 18
            Layout.fillWidth: true
            Layout.bottomMargin: lastGameCleared ? -24 : 0
        }
        TextField {
            id: textFieldName
            focus: true
            maximumLength: 10
            placeholderText: qsTr("Name")
            Layout.fillWidth: true
            visible: lastGameCleared
            validator: RegExpValidator { regExp: /\S+.*\S*/ }
            selectByMouse: true
            onTextChanged: {
                if (lastGameCleared) {
                    inputRecordDialog.standardButton(Dialog.Ok).enabled = textFieldName.acceptableInput && !recordManager.busy
                }
            }
            onAccepted: {
                if (textFieldName.acceptableInput) {
                    inputRecordDialog.accept()
                }
            }
        }
        CheckBox {
            id: checkBoxRememberUsername
            visible: lastGameCleared
            text: qsTr("Remember this username")
            enabled: textFieldName.acceptableInput && textFieldName.text !== arcade.username
            checked: textFieldName.text === arcade.username
            Layout.topMargin: -32
        }
    }

    onOpened: {
        if (lastGameCleared) {
            textFieldName.text = arcade.username
            textFieldName.selectAll()
            inputRecordDialog.standardButton(Dialog.Ok).enabled = textFieldName.acceptableInput
            if (inputRecordDialog.newScore && soundNewRecord.volume > 0.0) {
                soundNewRecord.play()
            } else if (soundWin.volume > 0.0) {
                soundWin.play()
            }
        } else {
            if (!soundLose.playing && soundLose.volume > 0.0) {
                soundLose.play()
            }
        }
        focus = true
        textFieldName.forceActiveFocus()
    }

    onClosed: {
        if (!lastGameCleared && result === Dialog.Accepted) {
            arcade.newGame()
        }
    }
    property int record: (boardFrontend.steps + 1)*1000 - arcade.recordMs
    onAccepted: {
        if (lastGameCleared && textFieldName.acceptableInput) {
            recordManager.addRecord(textFieldName.text, arcade.recordMs, record)
            if (checkBoxRememberUsername.checked) {
                arcade.username = textFieldName.text
            }
        }
    }
} // inputRecordDialog
