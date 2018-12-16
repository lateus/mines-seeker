import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: page

    property string name1;
    property string name2;
    property string name3;
    property string name4;
    property string name5;
    property string name6;
    property string name7;
    property string name8;
    property string name9;
    property string name10;

    property string time1;
    property string time2;
    property string time3;
    property string time4;
    property string time5;
    property string time6;
    property string time7;
    property string time8;
    property string time9;
    property string time10;

    property color textColor: "black"

    ColumnLayout {
        spacing: 5
        anchors.fill: parent
        anchors.bottomMargin: 8

        RecordEntry {
            id: entry1
            Layout.fillWidth: true

            rank: 1
            name: name1
            time: time1
            textColor: page.textColor
        }
        RecordEntry {
            id: entry2
            Layout.fillWidth: true

            rank: 2
            name: name2
            time: time2
            textColor: page.textColor
        }
        RecordEntry {
            id: entry3
            Layout.fillWidth: true

            rank: 3
            name: name3
            time: time3
            textColor: page.textColor
        }
        RecordEntry {
            id: entry4
            Layout.fillWidth: true

            rank: 4
            name: name4
            time: time4
            textColor: page.textColor
        }
        RecordEntry {
            id: entry5
            Layout.fillWidth: true

            rank: 5
            name: name5
            time: time5
            textColor: page.textColor
        }
        RecordEntry {
            id: entry6
            Layout.fillWidth: true

            rank: 6
            name: name6
            time: time6
            textColor: page.textColor
        }
        RecordEntry {
            id: entry7
            Layout.fillWidth: true

            rank: 7
            name: name7
            time: time7
            textColor: page.textColor
        }
        RecordEntry {
            id: entry8
            Layout.fillWidth: true

            rank: 8
            name: name8
            time: time8
            textColor: page.textColor
        }
        RecordEntry {
            id: entry9
            Layout.fillWidth: true

            rank: 9
            name: name9
            time: time9
            textColor: page.textColor
        }
        RecordEntry {
            id: entry10
            Layout.fillWidth: true

            rank: 10
            name: name10
            time: time10
            textColor: page.textColor
        }
    } // ColumnLayout
}
