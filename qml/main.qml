// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle{
    id: stenographer
    width: 600
    height: 400
    visible: true

    signal beginStopRec
    signal playLast(variant text)
    signal convertAndSent
    signal saveText(variant text,variant name)
    signal autoMod
    signal switchAutoMod
    signal setOverK (variant factor)
    signal setIT1 (variant factor)
    signal setITstep1 (variant factor)
    signal setIT2 (variant factor)
    signal setITstep2 (variant factor)
    signal setDelay (variant factor)
    signal getOptions;
    signal changeAudioDevice( variant iDev )
    signal setSaveTempFiles(variant is)

    signal startTest
    function stopTest(value) {
        calibration.level = value;
        calibration.is = true;
    }

    function stopRec() {
        stenographer.activated = false;
    }

    function updateData(text) {
        if(showAllResults.is)
            display.text = display.text + "\n" + text.split("\n").join("|");
        else
            display.text = display.text + "\n" + text.split("\n")[1];
    }  // slot
    function updateLog(text) {
        log.text = text + "\n" + log.text
    }  // slot

    function setAudioDevices(list){
        devices.model = list;
    }

    signal startDraw
    function setImage(url) { svgImage.source = url }

    function setVolumeRange(range) { volumeMeter.setRange(range); }
    function setVolumeValue(value) { volumeMeter.setValue(value); }

    property bool activated: false

    Column{
        id: column
        width: parent.width
        height: parent.height
        Row {
            id: row
            height: 30
            width: parent.width
            Button {
                id: manualMod
                operation: stenographer.activated ? qsTr("Stop")
                                                  : qsTr("Start")
                width: 70
                color:  stenographer.activated ? 'red'
                                               : ''
                onClicked: {
                    stenographer.activated = !stenographer.activated;
                    beginStopRec()
                }
            }
            Button {
                width: 100
                color: ''
                visible: stenographer.activated ? false
                                                : true
                operation: qsTr("Play")
                onClicked: playLast(display.text)
            }
            Button {
                width: 100
                color: ''
                visible: stenographer.activated ? false
                                                : true
                operation: qsTr("Recognize")
                onClicked: convertAndSent()
            }
            Button {
                width: 100
                color: ''
                visible: stenographer.activated ? false
                                                : true
                operation: qsTr("Save text")
                onClicked: saveText(display.text,"text.txt")
            }
            Button {
                width: 100
                color: ''
                visible: stenographer.activated ? false
                                                : true
                operation: qsTr("Options")
                onClicked: {
                    getOptions();
                    tabs.showWindow(optionsWindow)
                }
            }
            Button {
                anchors.right: parent.right
                width: 50
                color: ''
                operation: qsTr("Exit")
                onClicked: Qt.quit()
            }
        }

        Tabs{
            id:tabs
            width: parent.width
            height: column.height - row.height
            Window {
                visible: true
                id: mainWindow
                Row{
                    height: parent.height - log.height
                    width: parent.width
                    Stenographer {
                        text: "Стенограф: "
                        id: display
                        height: parent.height
                        width: parent.width - volumeMeter.width
                    }
                    Meter{
                        id: volumeMeter
                    }
                }
                Log {
                    Button {
                        anchors.bottom: parent.top
                        anchors.right: parent.right
                        width: 50
                        height: 15
                        color: ''
                        operation: qsTr("Show logs")
                        onClicked: {log.activated = !log.activated}
                    }
                    id: log
                    property bool activated: false
                    height: activated ? 70
                                      : 1
                }
            }
            Window {
                visible: false
                id: optionsWindow
                Button {
                    id: setAutoMod
                    property bool is: true
                    width: 150
                    color: is ? 'green'
                              : 'red'
                    operation: qsTr("Auto mod is %1").arg(is ? "on" : "off")
                    onClicked: {
                        switchAutoMod()
                        is = !is
                    }
                }
                Button {
                    id: showAllResults
                    property bool is: false
                    width: 150
                    color: is ? 'green'
                              : 'red'
                    operation: qsTr("Show all results %1").arg(is ? "on" : "off")
                    onClicked: {is = !is}
                }
                Button {
                    id: saveTempFiles
                    property bool is: false
                    width: 150
                    color: is ? 'green'
                              : 'red'
                    operation: qsTr("Save temp files %1").arg(is ? "on" : "off")
                    onClicked: {
                        is = !is;
                        setSaveTempFiles(is);
                    }
                }
                Button {
                    id: calibration
                    property bool is: true
                    property int level: 5000
                    width: 300
                    color: is ? 'green'
                              : 'red'
                    operation: is ? qsTr("Start calibration (current level is %1x%2)")
                                    .arg(level)
                                    .arg(overK.factor)
                                  : qsTr("Testing...")
                    onClicked: {
                        if(is) startTest();
                        is = false;
                    }
                }
                Row{
                    id: overK
                    property double factor: 2
                    Button {
                        width: 20
                        height: 20
                        color: ''
                        operation: qsTr("-")
                        onClicked: {
                            overK.factor -= 0.1;
                            setOverK(overK.factor);
                        }
                    }
                    Text {
                        font.pointSize: 10
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        width: 100
                        text: qsTr("Factor is %1").arg(overK.factor)
                    }
                    Button {
                        width: 20
                        height: 20
                        color: ''
                        operation: qsTr("+")
                        onClicked: {
                            overK.factor += 0.1;
                            setOverK(overK.factor);
                        }
                    }

                }
//                Spin{
//                    id: it1
//                    factor: 8
//                    onSet: { setIT1(it1.factor) }
//                    message: "First integration time is: %1 ms"
//                }
//                Spin{
//                    id: its1
//                    factor: 4
//                    onSet: { setITstep1(its1.factor) }
//                    message: "First integration step is: %1 ms"
//                }
                Spin{
                    id: it2
                    factor: 400
                    onSet: { setIT2(it2.factor) }
                    message: "Integration time is: %1 ms"
                }
                Spin{
                    id: its2
                    factor: 160
                    onSet: { setITstep2(its2.factor) }
                    message: "Integration step is: %1 ms"
                }
                Spin{
                    id: delay
                    factor: 1000
                    onSet: { setDelay(delay.factor) }
                    message: "Delay (buffer size): %1 ms"
                }

                Window{
                    visible: parent.visible
                    height: devices.contentHeight+30;
                    width: devices.contentWidth;
                    Text {
                        id: dev_text
                        font.pointSize: 14
                        text: qsTr("Audio input devices:")
                    }
                    ListView {
                        id: devices
                        anchors.bottom: parent.bottom
                        anchors.top: dev_text.bottom
                        focus: true
                        delegate: deviceDelegate
                        Component {
                            id: deviceDelegate
                            Button {
                                height: 20; width: 200
                                color: ListView.isCurrentItem ? "green" : ""
                                operation: qsTr(model.modelData)
                                onClicked: {
                                    updateLog("Set device: "+index);
                                    devices.currentIndex = index;
                                    changeAudioDevice(index);
                                }
                            }
                        }
                    }
                }
                Window {
                    visible: false
                    Text {
                        font.pointSize: 14
                        text: qsTr("Voice commands:")
                    }
                    Button {
                        id: startCommand
                        property bool is: true
                        width: 150
                        color: is ? 'green'
                                  : 'red'
                        operation: qsTr("Set start command")
                        onClicked: {is = !is}
                    }
                    Button {
                        id: stopCommand
                        property bool is: true
                        width: 150
                        color: is ? 'green'
                                  : 'red'
                        operation: qsTr("Set stop command")
                        onClicked: {is = !is}
                    }
                    Button {
                        id: saveCommand
                        property bool is: true
                        width: 150
                        color: is ? 'green'
                                  : 'red'
                        operation: qsTr("Set save file command")
                        onClicked: {is = !is}
                    }
                 }
            }
        }
    }
}
