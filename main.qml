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
//        model: DataStore.vtkSources
        textRole: "objectName"
        font { family: "Courier"; pointSize: 16 }
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
        anchors.fill: parent
        color: "lightBlue"
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
        anchors { fill: parent; margins: 35 }

        Vtk.Actor {

          Vtk.PolyDataMapper {

            Vtk.ConeSource {}
          }
        }
      }
    }
  }
}
