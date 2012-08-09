// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


Rectangle {
    id: displayRec
    width: parent.width

    height: 300

    property alias text: display.text

    Scrollable {
        contentWidth: display.width
        contentHeight: display.height
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        hideScrollBarsWhenStopped: false

        anchors.fill: parent

        TextEdit {
            id: display
            width: displayRec.width
            wrapMode: TextEdit.WordWrap
            font.pixelSize: 16
        }
    }
}
