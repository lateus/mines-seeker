import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    id: root
    modal: true
    width: 410
    height: 430
    title: qsTr("About Qt")
    standardButtons: Dialog.Close

    property string source: "qrc:/images/icons/QtProject-qtcreator.png"
    property string sourceSize: "128x128"

    Rectangle {
        id: rectangleIcon
        width: 128
        height: 128
        anchors.horizontalCenter: parent.horizontalCenter

        Ripple {
            id: ripple
            width: parent.width
            height: parent.height
            pressed: false
            color: "#AA41CD52"

            SequentialAnimation on pressed {
                loops: Animation.Infinite
                PropertyAnimation { from: false; to: true;  duration: 400 }
                PropertyAnimation { from: true;  to: false; duration: 200 }
            }
        }

        Image {
            id: imageAppIcon
            anchors.fill: parent
            source: root.source
            sourceSize: root.sourceSize
        }
    }

    ScrollView {
        id: scrollView
        clip: true
        anchors.topMargin: 8
        anchors.bottomMargin: -32
        anchors.top: rectangleIcon.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.horizontal.interactive: false
        ScrollBar.vertical.interactive: false

        Text {
            id: textChangelog
            width: scrollView.width
            height: scrollView.height
            onLinkActivated: { Qt.openUrlExternally(link) }
            text: qsTr("<p><b>This program uses Qt version %1.</b></p><br>").arg("5.11.0") +
                          qsTr("<p>Qt is a <i>C++ toolkit for cross-platform application " +
                               "development</i>.</p>" +
                               "<p>Qt provides single-source portability across all major desktop and mobile " +
                               "operating systems. It is also available for embedded Linux and other " +
                               "embedded operating systems.</p><br>" +
                               "<p>Qt offers both <i>commercial</i> and <i>opensource</i> licences. Please see <a href=\"http://%2/\">%2</a> " +
                               "for an overview of Qt licensing.</p><br>" +
                               "<p><i>Copyright © %1 The Qt Company Ltd</i> and other " +
                               "contributors. See <a href=\"http://%3/\">%3</a> for more information.</p><br>").arg("2018").arg("qt.io/licensing").arg("qt.io")
            wrapMode: Text.WordWrap
            font.pointSize: 12
        }
    }
}
