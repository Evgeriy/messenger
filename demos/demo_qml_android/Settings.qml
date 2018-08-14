import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    width: 480
    height: 600

    header: Label {
        id: headerChats
        height: 60

        RoundButton {
            background: Rectangle {
                color: "#dd0cbcbc"
                radius: 0
                opacity: 0.4
            }
            enabled: false
            width: parent.width
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
        }

        text: "Settings"
        font.capitalization: Font.MixedCase
        font.bold: true
        font.family: "Tahoma"
        font.pixelSize: 30
        color: "black"
        lineHeight: 0.4
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        padding: 10

    }

    Label {
        text: qsTr("You are on Page Settings")
        anchors.centerIn: parent
    }
}
