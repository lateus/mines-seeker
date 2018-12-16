import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: record
    color: "transparent"

    property int rank;
    property string name;
    property string time;
    property color textColor: "black"

    Text {
        id: ranking
        text: rank + '.'
        width: 30
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: time.left
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8

        color: textColor
    }

    Text {
        id: playerName
        text: name.length ? name : "----------"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: time.left
        anchors.rightMargin: 8
        anchors.left: ranking.right
        anchors.leftMargin: 8

        color: textColor
    }

    Text {
        id: playerTime
        text: name.length ? time : "--:--.---"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 8
        font.family: "Source Code Pro"
        font.weight: Font.DemiBold

        color: textColor
    }
}
