// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Row{
    signal set
    property double factor: 2
    property string message: "Value is: 1%"
    Button {
        width: 60
        height: 20
        color: ''
        operation: qsTr("-1000")
        onClicked: {
            if(parent.factor > 1000)
            parent.factor -= 1000;
            set(parent.factor);
        }
    }
    Button {
        width: 50
        height: 20
        color: ''
        operation: qsTr("-100")
        onClicked: {
            if(parent.factor > 100)
            parent.factor -= 100;
            set(parent.factor);
        }
    }
    Button {
        width: 40
        height: 20
        color: ''
        operation: qsTr("-10")
        onClicked: {
            if(parent.factor > 10)
            parent.factor -= 10;
            set(parent.factor);
        }
    }
    Button {
        width: 30
        height: 20
        color: ''
        operation: qsTr("-1")
        onClicked: {
            if(parent.factor > 1)
            parent.factor -= 1;
            set(parent.factor);
        }
    }
    Text {
        font.pointSize: 10
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: 230
        text: qsTr(parent.message).arg(parent.factor)
    }
    Button {
        width: 30
        height: 20
        color: ''
        operation: qsTr("+1")
        onClicked: {
            parent.factor += 1;
            set(parent.factor);
        }
    }
    Button {
        width: 40
        height: 20
        color: ''
        operation: qsTr("+10")
        onClicked: {
            parent.factor += 10;
            set(parent.factor);
        }
    }
    Button {
        width: 50
        height: 20
        color: ''
        operation: qsTr("+100")
        onClicked: {
            parent.factor += 100;
            set(parent.factor);
        }
    }
    Button {
        width: 60
        height: 20
        color: ''
        operation: qsTr("+1000")
        onClicked: {
            parent.factor += 1000;
            set(parent.factor);
        }
    }
}
