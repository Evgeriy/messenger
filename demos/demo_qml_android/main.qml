import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 480
    height: 640
    title: "Messenger"

    SwipeView {
        id: swipeView
        antialiasing: true
        interactive: true
        orientation: Qt.Horizontal
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Favorites {

        }

        Feed {

        }

        Chats {

        }

        Communities {

        }

        Settings {

        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            icon.width: 32
            icon.height: 32
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "./favorites.png"
        }
        TabButton {
            icon.width: 32
            icon.height: 32
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "./feed.png"
        }
        TabButton {
            icon.width: 32
            icon.height: 32
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "./chats.png"
        }
        TabButton {
            icon.width: 32
            icon.height: 32
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "./communities.png"
        }
        TabButton {
            opacity: 0.6
            icon.width: 32
            icon.height: 32
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "./settings.png"
        }
    }
}
