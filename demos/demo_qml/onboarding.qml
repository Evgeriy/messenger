import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Text {
        id: pageTitle
        width: 173
        height: 42
        text: qsTr("Messenger")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 30

        anchors {
            top: parent.top
            topMargin: 100
            horizontalCenter: parent.horizontalCenter
        }
    }

    Column {
        id: buttonsColumn

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        Button {
            id: registrationButton
            text: "Registration"

            onClicked: {
                mainWindow.title = "Registration"
            }
        }

        Button {
            id: authorizationButton
            text: "Authorization"

            onClicked: {
                mainWindow.title = "Authorization"
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
