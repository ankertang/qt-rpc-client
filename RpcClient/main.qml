import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import PM.JsonRpc 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Connections {
        target: JsonRpc
        onRpcReply: {
            console.log(id, reply)
            textArea.text = JSON.stringify(reply) + "\n" + textArea.text
        }
    }

    Rectangle {
        color: "#222222"
        x: 57
        y: 14
        width: 447
        height: 20

        TextInput {
            id: textInput
            anchors.fill: parent
            text: "thomas"
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            font.pixelSize: 12
            color: "#eeeeee"
        }
    }

    Text {
        id: element
        x: 10
        y: 14
        text: qsTr("Name:")
        font.pixelSize: 12
    }

    Button {
        id: button
        x: 523
        y: 14
        text: qsTr("Rpc Call")
        onClicked: {
            var jsonObj = {"Who": textInput.text}
            JsonRpc.rpcCall("CallService.Call", jsonObj)
        }
    }

    ScrollView {
        id: view
        x: 10
        y: 66
        width: 613
        height: 398
        TextArea {
            id: textArea
            anchors.fill: parent
            color: "#eeeeee"
            background: Rectangle {
                color: "#222222"
                anchors.fill: parent
            }
        }
    }
}
