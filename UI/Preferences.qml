import QtQuick 2.11
import QtQuick.Controls 2.4

Dialog {
    id: root
    modal: true
    title: qsTr("Preferences ")
    standardButtons: Dialog.Apply | Dialog.Reset | Dialog.Close
    clip: true

    property real initialVolume: 1.0;
    property bool initialOneClickReveal: false;
    property int  initialBoardGeneration: 0;

    onOpened: {
        initialVolume = settings.volume
        initialOneClickReveal = settings.oneClickRevealSiblings
        initialBoardGeneration = settings.boardGeneration
        updateButtonsStates()
    }

    onApplied: {
        settings.volume = sliderVolume.value
        settings.oneClickRevealSiblings = checkBoxOneClickReveal.checked
        settings.boardGeneration = comboBoxBoardGeneration.currentIndex
        updateButtonsStates()
        accept()
    }

    onReset: {
        sliderVolume.value = 1.0
        checkBoxOneClickReveal.checked = false
        comboBoxBoardGeneration.currentIndex = 0
        updateButtonsStates()
    }

    function updateButtonsStates() {
        standardButton(Dialog.Apply).enabled = ( (initialVolume !== sliderVolume.value) || (initialOneClickReveal !== checkBoxOneClickReveal.checked) || (initialBoardGeneration !== comboBoxBoardGeneration.currentIndex) )
        standardButton(Dialog.Reset).enabled = ( (sliderVolume.value !== 1.0) || (checkBoxOneClickReveal.checked !== false) || (comboBoxBoardGeneration.currentIndex !== 0) )
    }

    GroupBox {
        id: groupBoxVolume
        height: 110
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        title: qsTr("Volume settings")

        Text {
            id: textSoundEffects
            text: qsTr("Sound effects")
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 8
            font.pixelSize: 12
        }

        ToolButton {
            id: toolButtonMute
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.top: textSoundEffects.bottom
            display: AbstractButton.IconOnly
            icon.source: "qrc:/images/arcade/Arcade/ic_volume_" + ( checked ? "up" : "off" ) + "_24px.svg"
            checkable: true
            checked: enabled
            enabled: sliderVolume.previousValue > 0.0

            onCheckedChanged: {
                if (checked && !sliderVolume.pressed) {
                    sliderVolume.value = sliderVolume.previousValue
                } else if (!checked && sliderVolume.value > 0.0) {
                    sliderVolume.previousValue = sliderVolume.value
                    sliderVolume.value = 0.0
                }
                updateButtonsStates()
            }
        }

        Slider {
            id: sliderVolume
            height: toolButtonMute.height
            anchors.rightMargin: 8
            anchors.right: parent.right
            anchors.verticalCenter: toolButtonMute.verticalCenter
            anchors.left: toolButtonMute.right
            anchors.leftMargin: 8
            value: settings.volume
            stepSize: 0.01

            ToolTip {
                parent: sliderVolume.handle
                visible: sliderVolume.pressed
                text: ~~(sliderVolume.value*100)
            }

            Behavior on value { NumberAnimation { duration: 100; easing.type: Easing.InOutQuad } }

            onValueChanged: {
                if (toolButtonMute.checked || ( !toolButtonMute.enabled && (value > 0.0))) {
                    previousValue = value
                }
                updateButtonsStates()
            }

            property real previousValue: settings.volume
        }
    }

    GroupBox {
        id: groupBoxArcade
        height: 180
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: groupBoxVolume.bottom
        anchors.topMargin: 16
        title: qsTr("Arcade")

        CheckBox {
            id: checkBoxOneClickReveal
            text: qsTr("Reveal siblings with one left-click")
            anchors.rightMargin: 8
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 8

            checked: settings.oneClickRevealSiblings

            onCheckedChanged: {
                updateButtonsStates()
            }
        }

        ComboBox {
            id: comboBoxBoardGeneration
            anchors.rightMargin: 8
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.top: textBoardGeneration.bottom

            currentIndex: settings.boardGeneration

            onCurrentIndexChanged: {
                updateButtonsStates()
            }

            model: [qsTr("First click never hits a mine"), qsTr("Totally random")]
        }

        Text {
            id: textBoardGeneration
            text: qsTr("Board generation")
            anchors.leftMargin: 8
            anchors.topMargin: 8
            anchors.left: parent.left
            anchors.top: checkBoxOneClickReveal.bottom
            font.pixelSize: 12
        }
    }

}
