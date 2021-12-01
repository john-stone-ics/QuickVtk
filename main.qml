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
  Vtk.SphereSource {
    id: src2
    objectName: "Vtk.SphereSource"
    thetaResolution: 15
    phiResolution: 8
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
        id: source
        Layout.fillHeight: true
        Layout.preferredWidth: childrenRect.width*1.055
        model: [
          { label: "ConeSource",     source: src1 },
          { label: "SphereSource",   source: src2 }
        ]
        textRole: "label"
        valueRole: "source"
        font { family: "Courier"; pointSize: 16 }
      }
    }
  }

  DynamicSplitView {
    anchors.fill: parent

    itemDelegate: DynamicSplitView.ItemDelegate
    {
      Rectangle {
        id: bg
        anchors.fill: parent
        color: "steelblue"
        border {
          color: viewer.selected ? Qt.darker(bg.color) : Qt.lighter(bg.color);
          width: 35
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
          anchors { fill: parent; margins: bg.border.width }

          property bool selected: false
          onClicked: selected ^= 1

          Vtk.Actor {
            Vtk.PolyDataMapper {
              input: source.currentValue
            }
          }
        }
      }
    }
  }
}
