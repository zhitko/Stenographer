// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import QtDesktop 0.1

import 'qml'

Rectangle{
    id: mainWindow
    signal beginStopRec
    signal playLast
    signal convertAndSent
    width: 600
    height: 400

    function updateData(text) { display.text = text }  // slot
    function updateLog(text) { log.text = text + "\n" + log.text}  // slot
    visible: true

    Column{
        id: column
        width: parent.width
        height: parent.height
        Row {
            id: row
            height: 30
            width: parent.width
            Button {
                width: 70
                //height: row.height
                color: 'purple'
                operation: "Запись"
                onClicked: beginStopRec()
            }
            Button {
                width: 130
                //height: row.height
                color: 'purple'
                operation: "Воспроизвести"
                onClicked: playLast()
            }
            Button {
                width: 100
                //height: row.height
                color: 'purple'
                operation: "Отправить"
                onClicked: convertAndSent()
            }
            Button {
                width: 100
                //height: row.height
                color: 'purple'
                operation: "Настройка"
                //onClicked: convertAndSent()
            }
            Button {
                width: 80
                //height: row.height
                color: 'purple'
                operation: "Выход"
                onClicked: Qt.quit()
            }
        }
        Rectangle {
            id: displayRec
            width: parent.width
            height: parent.height - row.height - logRec.height
            Scrollable {
                contentWidth: display.width
                contentHeight: display.height
                flickableDirection: Flickable.HorizontalAndVerticalFlick
                hideScrollBarsWhenStopped: false

                anchors.fill: parent

                TextEdit {
                    id: display
                    width: column.width
                    wrapMode: TextEdit.WordWrap
                }
            }
        }
        Rectangle {
            id: logRec
            width: parent.width
            height: 50
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
                    width: column.width
                    wrapMode: TextEdit.WordWrap
                }
            }
        }
    }
}
