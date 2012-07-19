// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: logRec
    width: parent.width
    height: 50
    property alias text: log.text

    Scrollable {
        color: 'black'
        contentWidth: log.width
        contentHeight: log.height
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        hideScrollBarsWhenStopped: false

        anchors.fill: parent

        TextEdit {
            id: log
            color: 'green'
            width: logRec.width
            wrapMode: TextEdit.WordWrap
        }
    }
}
