import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: window
    objectName: "window"

    width: 1024
    height: 768
    visible: true
    title: qsTr("Splitter Test")

    readonly property var colors: ["red", "green", "blue", "cyan", "magenta", "yellow", "white", "black"]

    DynamicSplitView {
        id: rsv
        anchors.fill: parent

        itemDelegate: DynamicSplitView.ItemDelegate
        {
            Rectangle {
                anchors.fill: parent
                color: colors[myIID % colors.length]
                border.width: 2
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
                onClicked: {
                    if (mouse.button === Qt.LeftButton) {
                        split(Qt.Horizontal)
                    } else if (mouse.button === Qt.RightButton) {
                        split(Qt.Vertical)
                    } else {
                        unsplit()
                    }
                }
            }

            Text {
                anchors.centerIn: parent
                text: mySplitView ? mySplitView.objectName : "<constructing>"
                font { pointSize: 16 }
            }
        }
    }
}
