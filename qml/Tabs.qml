// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Row {
    id: tabs
    width: parent.width
    height: parent.height

    property QtObject activeObject: null
    property QtObject preActiveObject: null

    property int _active: 0
    property int _preactive: 0

    function showWindow(ids){
        if(activeObject === ids){
            var tmp = tabs.activeObject;
            tabs.activeObject = tabs.preActiveObject;
            tabs.preActiveObject = tmp;
            tmp = tabs._active;
            tabs._active = tabs._preactive;
            tabs._preactive = tmp;
            tabs.setActive(tabs._active);
            tabs.setDisactive(tabs._preactive);
            return;
        }
        var count = tabs.children.length;
        //console.log(count);
        for(var i = 0; i<count; i++){
            if( tabs.children[i].visible ){
                tabs.preActiveObject = tabs.children[i];
                tabs._preactive = i;
            }
            tabs.setDisactive(i);
            if( tabs.children[i] === ids ){
                tabs.activeObject = ids;
                tabs.setActive(i);
                tabs._active = i;
            }
        }
    }
    function setActive(num){
        tabs.children[num].visible = true;
        tabs.children[num].width = tabs.width;
    }
    function setDisactive(num){
        tabs.children[num].visible = false;
        tabs.children[num].width = 0;
    }
}
