import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "dynamicsplitview"

import Vtk 1.0 as Vtk

ApplicationWindow {
  id: win
  visible: true
  width: 1024
  height: 768
  title: qsTr("Splitter Demo")

  Vtk.ConeSource {
    id: src1
    objectName: "Vtk.ConeSource"
    resolution: 25
    angle: 26
  }
  Vtk.CylinderSource {
    id: src2
    objectName: "Vtk.CylinderSource"
    resolution: 15
  }


  header: ToolBar {
    id: header
    RowLayout {
      anchors.fill: parent
      Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
      }
      Rectangle {
        color: "black"
        Layout.preferredWidth: 1
        Layout.fillHeight: true
      }
      Text {
        Layout.leftMargin: 10
        text: "vtkSource:"
        font { family: "Courier"; pointSize: 16 }
      }
      ComboBox {
        id: vtksrc
        Layout.fillHeight: true
        Layout.preferredWidth: childrenRect.width*1.055
        model: [
          { label: "ConeSource", source: src1 },
          { label: "CylinderSource", source: src2 }
        ]
        textRole: "label"
        valueRole: "source"
        font { family: "Courier"; pointSize: 16 }
        onCurrentValueChanged: console.log("onCurrentValueChanged", this)
      }
    }
  }

  DynamicSplitView {
    id: dsv
    anchors.fill: parent
    property int _selectedIID: -1

    itemDelegate: DynamicSplitView.ItemDelegate
    {
      Rectangle {
        id: bg
        anchors.fill: parent
        color: "steelblue"
        border { color: viewer.selected ? Qt.darker(bg.color) : Qt.lighter(bg.color); width: 35 }
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

      Vtk.Viewer {
        id: viewer
        anchors { fill: bg; margins: bg.border.width }

        property bool selected: false
        onClicked: selected ^= 1

        Vtk.Actor {
          Vtk.PolyDataMapper {
            id: polyDataMapper
            input: vtksrc.currentValue
            onInputChanged: console.log("onInputChanged", this, input[0] ? input[0].objectName : "nope")
          }
      }
    }

    SequentialAnimation {
      id: resolutionAnimation
      loops: Animation.Infinite
      running: false
      NumberAnimation {
        target: vtksrc.currentValue
        property: "resolution"
        from: 25; to: 50
        easing.type: Easing.Linear; duration: 2000
      }
      NumberAnimation {
        target: vtksrc.currentValue
        property: "resolution"
        from: 50; to: 5
        easing.type: Easing.Linear; duration: 2000
      }
      NumberAnimation {
        target: vtksrc.currentValue
        property: "resolution"
        from: 5; to: 25
        easing.type: Easing.Linear; duration: 2000
      }
    }

    SequentialAnimation {
      id: radiusAnimation
      loops: Animation.Infinite
      running: false
      NumberAnimation {
        target: vtksrc.currentValue
        property: "radius"
        from: 0.5; to: 1.0
        easing.type: Easing.Linear; duration: 2000
      }
      NumberAnimation {
        target: vtksrc.currentValue
        property: "radius"
        from: 1.0; to: 0.1
        easing.type: Easing.Linear; duration: 2000
      }
      NumberAnimation {
        target: vtksrc.currentValue
        property: "radius"
        from: 0.1; to: 0.5
        easing.type: Easing.Linear; duration: 2000
      }
    }

    SequentialAnimation {
      id: heightAnimation
      loops: Animation.Infinite
      running: false
      NumberAnimation {
        target: vtksrc.currentValue
        property: "height"
        from: 1.0; to: 2.0
        easing.type: Easing.Linear; duration: 2000
      }
      NumberAnimation {
        target: vtksrc.currentValue
        property: "height"
        from: 2.0; to: 0.25
        easing.type: Easing.Linear; duration: 2000
      }
      NumberAnimation {
        target: vtksrc.currentValue
        property: "height"
        from: 0.25; to: 1.0
        easing.type: Easing.Linear; duration: 2000
      }
    }

    GroupBox {
      title: "Animations"

      anchors { top: parent.top; topMargin: viewer.anchors.margins; right: parent.right; rightMargin: viewer.anchors.margins }
      Column {
        width: childrenRect.width
        height: childrenRect.height

        Repeater {
          id: repeater
          model: [
            { label: "resolution", item: resolutionAnimation },
            { label: "radius",     item: radiusAnimation },
            { label: "height",     item: heightAnimation }
          ]
          delegate: CheckBox {
            text: modelData.label
            checked: modelData.item.running
            onToggled: modelData.item.running ^= 1
          }
          data: [
            Connections {
              target: vtksrc
              function onCurrentValueChanged()  {
                console.log("vtksrc.currentValueChanged")
                for(let i=0,nI=repeater.count; i<nI; ++i)
                  repeater.model[i].item.running = false;
              }
            }
          ]
        }
      }
    }

//            [
//          ListView {
//          anchors { top: parent.top; right: parent.right }
//          model: ListModel {
//            ListElement { label: "resolution"; item: resolutionAnimation }
//            ListElement { label: "angle";      item: angleAnimation }
//            ListElement { label: "height";     item: heightAnimation }
//          }
//          delegate: CheckDelegate {
//            required property string label
//            required property var item
//            text: label
//            checked: item.running
//            onToggled: item.running ^= 1
//          }
//        }
//        ]
//      }

    }
  }
}
