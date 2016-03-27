import QtQuick 2.5

Item {
    id: messageItem
    width: 559
    anchors.left: parent.left
    implicitHeight: messageCont.implicitHeight + 20

    Image {
        source: imageSrc
        id: avatar
        width: 70
        height: 70
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    Rectangle {
        id: messageCont
        width: 457
        implicitHeight: replyUsername.implicitHeight + replyText.implicitHeight + 20
        color: "#16d2ff"
        radius: 10
        anchors.left: avatar.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea {
            id: mouseAreaMsg
            anchors.fill: parent
            property bool ipVisible: false
            onClicked: {
                if (!ipVisible) {
                    fadeIn.start()
                    expand.start()
                    ipVisible = true
                } else {
                    fadeOut.start()
                    contract.start()
                    ipVisible = false
                }
            }
        }

        Text {
            text: userName
            id: replyUsername
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 5
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            font.pixelSize: 14
            font.weight: Font.Bold
        }

        Text {
            text: textContents
            id: replyText
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.topMargin: 25
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.rightMargin: 10
            font.pixelSize: 12
        }
    }

    Text {
        text: ipAddress
        id: ipAddressText
        anchors.top: messageCont.bottom
        anchors.topMargin: 5
        anchors.left: messageCont.left
        anchors.leftMargin: 5
        font.pixelSize: 9
        opacity: 0.0
        NumberAnimation {
            id: fadeIn
            target: ipAddressText
            properties: "opacity"
            from: 0.0
            to: 1.0
            duration: 100
        }
        NumberAnimation {
            id: fadeOut
            target: ipAddressText
            properties: "opacity"
            from: 1.0
            to: 0.0
            duration: 100
        }

        PropertyAnimation {
            id: expand
            target: messageItem
            properties: "implicitHeight"
            to: messageCont.implicitHeight + 40
            duration: 100
        }
        PropertyAnimation {
            id: contract
            target: messageItem
            properties: "implicitHeight"
            to: messageCont.implicitHeight + 20
            duration: 100
        }
    }
}

/*

*/

/*Item {
    id: messageItem
    width: 559
    height: childrenRect + 20
    anchors.left: parent

    Image {
        id: avatar
        width: 70
        height: 70
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    Rectangle {
        id: messageCont
        width: 457
        height: childrenRect + 20
        color: "#85e9fe"
        radius: 10
        anchors.right: avatar.left
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
        }

        Text {
            id: replyText
            anchors.fill: parent
            anchors.margins: 10
            text: qsTr("Text")
            font.pixelSize: 12
        }
    }
}*/
