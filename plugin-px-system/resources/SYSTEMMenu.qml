import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3

RowLayout {
      id: rowLayout
      anchors.fill: parent
  TextField {
        placeholderText: "This wants to grow horizontally"
        Layout.fillWidth: true
      }
  Button {
    text: "Button"
  }
}