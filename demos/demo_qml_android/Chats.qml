import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

Page {
    id: page
    width: 480
    height: 600

    header: Label {
        id: headerChats
        height: 60

        RoundButton {
            background: Rectangle {
                color: "#dd0cbcbc"
                radius: 0
                opacity: 0.3
            }
            enabled: false
            width: parent.width
            height: parent.height
            focusPolicy: Qt.NoFocus
            anchors.horizontalCenter: parent.horizontalCenter
        }

        text: "Chats"
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

    ListView {
        id: listView

        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 14
        anchors.fill: parent

        flickDeceleration: 1505
        antialiasing: true
        highlightRangeMode: ListView.NoHighlightRange
        snapMode: ListView.NoSnap
        orientation: ListView.Vertical
        flickableDirection: Flickable.VerticalFlick

        delegate: Item {
            x: 10
            width: parent.width
            height: 80



            Row {
                id: row1
                width: parent.width
                height: parent.height

                Column {
                    id: col1
                    width: row1.height / 1.2

                    RoundButton {
                        background: Rectangle {
                            color: colorCode
                            border.color: "#dd0cbcbc"
                            border.width: readed ? 4 : 0
                            radius: 100
                            opacity: readed ? 0.2 : 0.8
                        }
                        enabled: readed
                        width: row1.height / 1.2
                        height: row1.height / 1.2
                    }
                }

                Column {
                    id: col2
                    width: row1.width - (col1.width + col3.width + col4.width) - row1.spacing * 4

                    Text {
                        id: communityTextBloc
                        text: communityName
                        font.family: "Tahoma"
                        font.pixelSize: 18
                        font.bold: true
                    }

                    Text {
                        id: charNameBloc
                        text: chatName
                        font.family: "Tahoma"
                        font.pixelSize: 15
                        font.bold: true
                    }

                    Text {
                        id: messagePartBloc
                        text: messagePart
                        width: parent.width
                        font.family: "Tahoma"
                        font.pixelSize: 15
                    }

                    spacing: 5
                }

                Column {
                    id: col3
                    width: row1.width / 6

                    RoundButton {
                        background: Rectangle {
                            border.width: tag ? 2 : 0
                            border.color: "#dd0cbcbc"
                            radius: 100
                            opacity: 0.3
                        }

                        text: tag
                        font.pixelSize: 10
                        enabled: true
                        width: row1.width / 7
                        height: row1.height / 3
                    }

                    spacing: 5
                }


                Column {
                    id: col4
                    width: row1.width / 9

                    Text {
                        id: timeBloc
                        text: time
                        font.family: "Tahoma"
                        font.pixelSize: 10
                    }

                    RoundButton {

                        background: Rectangle {
                            color: "#dd0cbcbc"
                            radius: 100
                            opacity: 0.3
                        }
                        font.family: "Tahoma"
                        font.pixelSize: 8
                        text: unreadedMsgCount
                        enabled: true
                        width: row1.height / 2.4
                        height: row1.height / 2.4
                    }

                    spacing: 7
                }

                spacing: 20
            }
        }

        model: ListModel {

            ListElement {
                readed: false
                communityName: "Work"
                chatName: "Masha"
                tag: "ma3ka"
                messagePart: "Я написала документацию..."
                colorCode: "cyan"
                time: "10:30"
                unreadedMsgCount: "34"
            }
            ListElement {
                readed: true
                communityName: "Work"
                chatName: "Pasha"
                tag: "cgf"
                messagePart: "Все работает!"
                colorCode: "blue"
                time: "00:30"
                unreadedMsgCount: "34"
            }
            ListElement {
                readed: true
                communityName: "Work"
                chatName: "Anton"
                tag: "ma3ka_dev"
                messagePart: "Скоро выложу релиз..."
                colorCode: "green"
                time: "12:30"
                unreadedMsgCount: "34"
            }
            ListElement {
                readed: false
                communityName: "Study"
                chatName: "Bot"
                tag: "ai"
                messagePart: "Если вы столкнулись с..."
                colorCode: "red"
                time: "10:09"
                unreadedMsgCount: "34"
            }
            ListElement {
                communityName: "Hobbies"
                chatName: "Bot"
                tag: "fav"
                messagePart: "Для того, чтобы впервы.."
                colorCode: "black"
                time: "10:30"
                unreadedMsgCount: "34"
            }
            ListElement {
                communityName: "Work"
                chatName: "Masha"
                messagePart: "Я написала документацию..."
                colorCode: "white"
                time: "10:43"
                unreadedMsgCount: "34"
            }
            ListElement {
                communityName: "Work"
                chatName: "Masha"
                tag: "home"
                messagePart: "Я написала документацию..."
                colorCode: "yellow"
                time: "10:30"
                unreadedMsgCount: "34"
            }
            ListElement {
                communityName: "Work"
                chatName: "Masha"
                tag: "sports"
                messagePart: "Я написала документацию..."
                colorCode: "orange"
                time: "10:30"
                unreadedMsgCount: "34"
            }
            ListElement {
                communityName: "Work"
                chatName: "Masha"
                tag: "trash"
                messagePart: "Я написала документацию..."
                colorCode: "sky"
                time: "10:30"
                unreadedMsgCount: "34"
            }
            ListElement {
                communityName: "Work"
                chatName: "Masha"
                tag: "news"
                messagePart: "Я написала документацию..."
                colorCode: "cyan"
                time: "10:30"
                unreadedMsgCount: "34"
            }
        }
    }
}

/*##^## Designer {
    D{i:35;anchors_height:160;anchors_width:110;anchors_x:254;anchors_y:89}
}
 ##^##*/
