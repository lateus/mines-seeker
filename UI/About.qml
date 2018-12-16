import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Dialog {
    id: root
    modal: true
    implicitWidth: 420
    implicitHeight: parent.height * 0.9
    title: qsTr("About ") + Qt.application.name
    standardButtons: Dialog.Close

    onOpened: {
        if (imageAppIcon.once) {
            rectangleIcon.opacity = 0.0
            imageAppIcon.once = false
        }
    }

    property string source: "qrc:/images/icons/appIcon.png"
    property string sourceSize: "128x128"


    property bool changelogFirst: false
    property bool simple: false
    property string headerString: "<b>The Crowporation</b> presents:<br>" +
                                  "<b>" + Qt.application.name + " " + Qt.application.version + "</b>" +
                                  "<br><i>Copyright © 2018 Carlos Enrique Pérez Sánchez.</i>"
    property string aboutText: qsTr("<i>This program is under the <b>GPL v2</b>.</i><br>" +
                                    "This program is distributed in the hope that it will be useful," +
                                    " but <b><i>without any warranty</i></b>; without even the implied warranty of " +
                                    "<b><i>merchantability</i></b> or <b><i>fitness for a particular purpose</i></b>. " +
                                    "See the <i>GNU General Public License</i> for more details.<br><br>" +
                                    "You should have received a copy of the <i>GNU General Public License</i> " +
                                    "along with this program. " +
                                    "If not, see <a href=\"%1\">%1</a>.<br>").arg("http://www.gnu.org/licenses/") +
                               qsTr("<br><b>Contact info: </b><i><a href=\"%1\">thecrowporation@gmail.com</a>.</i><br>").arg("mailto:thecrowporation@gmail.com")
    property string changelogText: qsTr("<b>v1.0</b> [Oct 10, 2018]<br>" +
                                        "<b>Initial version</b><br>" +
                                        "<i>   Rmv:</i> <u>Removed cryptographical protection from the database due to bugs</u><br>" +
                                        "<i>   New:</i> Added mines counter<br>" +
                                        "<i>   New:</i> Added a progress bar<br>" +
                                        "<i>   New:</i> Added internationalization support<br>" +
                                        "<i>   New:</i> Added spanish translation<br>" +
                                        "<i>   New:</i> Added splash screen<br>" +
                                        "<i>   New:</i> Added more sound effects<br>" +
                                        "<i>   New:</i> Added a busy indicator<br>" +
                                        "<i>   New:</i> Added <i>Preferences</i> menu<br>" +
                                        "<i>   New:</i> Added option to reveal siblings with a single click<br>" +
                                        "<i>   New:</i> Added company logo<br>" +
                                        "<i>   New:</i> Now the database access is asynchronous<br>" +
                                        "<i>   New:</i> Improved accesibility<br>" +
                                        "<i>   New:</i> Improved GUI and animations<br>" +
                                        "<i>   New:</i> Improved multimedia support<br>" +
                                        "<i>   New:</i> Improved UI performance<br>" +
                                        "<i>   Fxd:</i> Fixed a board bug that causes an overflow<br>" +
                                        "<i>   Fxd:</i> Minor bug fixes<br>" +
                                        "<br>") +
                                   qsTr("<b>v0.3</b>a [Oct 4, 2018]<br>" +
                                        "<b>Third <i>alpha</i> version</b><br>" +
                                        "<i>   New:</i> Score database is now cryptographically signed<br>" +
                                        "<br>") +
                                   qsTr("<b>v0.2</b>a [Oct 2, 2018]<br>" +
                                        "<b>Second <i>alpha</i> version</b><br>" +
                                        "<i>   New:</i> Drastically improved GUI and animations<br>" +
                                        "<i>   New:</i> Added scores<br>" +
                                        "<i>   New:</i> Save and restore the application state<br>" +
                                        "<i>   New:</i> Added sound effects<br>" +
                                        "<i>   Fxd:</i> Several bug fixes<br>" +
                                        "<br>") +
                                   qsTr("<b>v0.1</b>a [Sept 24, 2018]<br>" +
                                        "<b>First <i>alpha</i> version</b><br>")
    property string greetingsText: qsTr("Special thanks to <i>Edel</i>, who figure out how to discover surrounded cells and " +
                                        "how to resize the board properly.<br>" +
                                        "Thanks to the users <i>littlerobotsoundfactory</i>, <i>fins</i>, and <i>mativve</i> " +
                                        "from <a href=\"%1\">freesound.com</a> for their free fanzy sounds effects.<br>").arg("http://www.freesound.com")

    Rectangle {
        id: rectangleIcon
        width: 128
        height: 128
        anchors.horizontalCenter: parent.horizontalCenter
        Behavior on opacity {
            NumberAnimation { duration: 3000 }
        }

        onOpacityChanged: {
            if (opacity === 0.0) {
                imageAppIcon.source = root.source
                ripple.color = "#AA2196F3"
                opacity = 1.0
            }
        }

        Ripple {
            id: ripple
            width: parent.width
            height: parent.height
            pressed: false
            color: Material.rippleColor

            SequentialAnimation on pressed {
                loops: Animation.Infinite
                PropertyAction { value: true }
                PauseAnimation { duration: 250 }
                PropertyAction { value: false }
                PauseAnimation { duration: 400 }
            }
        }

        Image {
            id: imageAppIcon
            anchors.fill: parent
            source: "qrc:/images/Logo512.png"
            sourceSize: root.sourceSize
            property bool once: true
        }
    }

    Text {
        id: textCopyright
        text: headerString
        anchors.top: rectangleIcon.bottom
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.left: parent.left
        font.pointSize: 12
        clip: true
        wrapMode: Text.WordWrap
        onLinkActivated: { Qt.openUrlExternally(link) }
    }

    TabBar {
        id: tabBar
        anchors.top: textCopyright.bottom
        anchors.topMargin: 16
        anchors.right: parent.right
        anchors.left: parent.left
        visible: !simple

        currentIndex: swipeView.currentIndex
        onCurrentIndexChanged: {
            if (currentIndex != swipeView.currentIndex) {
                swipeView.currentIndex = currentIndex
            }
        }

        TabButton {
            id: tabButtonAbout
            text: qsTr("About")

            onClicked: {
                swipeView.currentIndex = 0
            }
        }

        TabButton {
            id: tabButtonChangelog
            text: qsTr("Changelog")

            onClicked: {
                swipeView.currentIndex = 1
            }
        }

        TabButton {
            id: tabButtonGreetings
            text: qsTr("Greetings")

            onClicked: {
                swipeView.currentIndex = 2
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -32
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: tabBar.bottom
        anchors.topMargin: simple ? -tabBar.height : 8
        interactive: !simple
        clip: true

        currentIndex: changelogFirst && !simple ? 1 : 0

        Item {
            id: pageAbout
            clip: true

            ScrollView {
                id: scrollViewAbout
                anchors.fill: parent
                clip: true
                contentWidth: width
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy:   ScrollBar.AlwaysOn

                Text {
                    id: textAbout
                    text: aboutText
                    anchors.fill: parent
                    font.pointSize: 12
                    clip: true
                    wrapMode: Text.WordWrap
                    onLinkActivated: { Qt.openUrlExternally(link) }
                }
            }
        }

        Item {
            id: pageChangelog
            clip: true
            visible: !simple

            ScrollView {
                id: scrollViewChangelog
                anchors.fill: parent
                clip: true
                contentWidth: width
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy:   ScrollBar.AlwaysOn

                Text {
                    id: textChangelog
                    text: changelogText
                    anchors.fill: parent
                    font.pointSize: 12
                    clip: true
                    wrapMode: Text.WordWrap
                    onLinkActivated: { Qt.openUrlExternally(link) }
                }
            }
        }

        Item {
            id: pageGreetings
            clip: true
            visible: !simple

            ScrollView {
                id: scrollViewGreetings
                anchors.fill: parent
                clip: true
                contentWidth: width
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy:   ScrollBar.AlwaysOn

                Text {
                    id: textGreetings
                    text: greetingsText
                    anchors.fill: parent
                    font.pointSize: 12
                    clip: true
                    wrapMode: Text.WordWrap
                    onLinkActivated: { Qt.openUrlExternally(link) }
                }
            }
        }
    }

}
