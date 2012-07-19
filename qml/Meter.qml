// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {

    width: 10
    height: parent.height
    color: 'white'

    function setRange(range){
        meter.range = range;
    }

    function setValue(value){
        meter.value = value;
    }

    Rectangle {
        id: meter
        property int range: 3
        property int value: 0
        color: 'red'
        width: parent.width
        height: parent.height/range*value
        anchors.bottom: parent.bottom
    }
}
