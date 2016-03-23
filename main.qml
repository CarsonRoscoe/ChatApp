import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Window {
    id: mainWindow
    visible: true
    width: 900
    height: 600
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.CustomizeWindowHint
    color: "transparent"

    Rectangle {
        id: mainContainer
        anchors.fill: parent
        radius: 3
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c1fef7"
            }

            GradientStop {
                position: 1
                color: "#00a9a2"
            }
        }

        Image {
            source:"images/window_controls/blue/close.png"
            id: close
            x: 552
            width: 16
            height: 16
            z: 3
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.top: parent.top
            anchors.topMargin: 8
            fillMode: Image.PreserveAspectCrop
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Qt.quit();
                }
            }
        }

        Rectangle {
            id: closeBackFill
            x: 578
            width: 14
            height: 14
            z: 2
            color: "#fff"
            anchors.rightMargin: 1
            anchors.topMargin: 1
            anchors.top: close.top
            anchors.right: close.right
        }

        Image {
            source:"images/window_controls/blue/maximize.png"
            id: maximize
            x: 535
            width: 16
            height: 16
            z: 3
            anchors.right: close.left
            anchors.rightMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 8
            fillMode: Image.PreserveAspectCrop
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    restore.visible = true
                    maximize.visible = false
                    mainWindow.showMaximized();
                }
            }
        }

        Rectangle {
            id: maximizeBackFill
            x: 578
            width: 14
            height: 14
            z: 2
            color: "#fff"
            anchors.rightMargin: 1
            anchors.topMargin: 1
            anchors.top: maximize.top
            anchors.right: maximize.right
        }

        Image {
            source:"images/window_controls/blue/restore.png"
            id: restore
            x: 535
            width: 16
            height: 16
            z: 3
            anchors.right: close.left
            anchors.rightMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 8
            fillMode: Image.PreserveAspectCrop
            visible: false
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    restore.visible = false
                    maximize.visible = true
                    mainWindow.showNormal();
                }
            }
        }

        Rectangle {
            id: restoreBackFill
            x: 578
            width: 14
            height: 14
            z: 2
            color: "#fff"
            anchors.rightMargin: 1
            anchors.topMargin: 1
            anchors.top: restore.top
            anchors.right: restore.right
        }

        Image {
            source:"images/window_controls/blue/minimize.png"
            id: minimize
            x: 518
            width: 16
            height: 16
            z: 3
            anchors.right: maximize.left
            anchors.rightMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 8
            fillMode: Image.PreserveAspectCrop
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mainWindow.showMinimized();
                }
            }
        }

        Rectangle {
            id: minimizeBackFill
            x: 578
            width: 14
            height: 14
            z: 2
            color: "#fff"
            anchors.rightMargin: 1
            anchors.topMargin: 1
            anchors.top: minimize.top
            anchors.right: minimize.right
        }

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            z: 1
            property variant clickPos: "1,1"
            property variant appStartPos: "1,1"
            onPressed: {
                // fetch global position due to bug on ubuntu
                clickPos = gApplicationController.getCursorPos()
                appStartPos = Qt.point(mainWindow.x, mainWindow.y)
            }

            onPositionChanged: {
                var newPos = gApplicationController.getCursorPos()
                var delta = Qt.point(newPos.x-clickPos.x, newPos.y-clickPos.y)
                mainWindow.x = appStartPos.x+delta.x;
                mainWindow.y = appStartPos.y+delta.y;
            }
        }

        Rectangle {
            id: chatContainer
            x: 270
            width: 600
            height: 510
            radius: 10
            z: 2
            anchors.top: parent.top
            anchors.topMargin: 44
            anchors.right: parent.right
            anchors.rightMargin: 30
            border.color: "#0954a0"
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#ffffff"
                }

                GradientStop {
                    position: 0.067
                    color: "#01c7c0"
                }

                GradientStop {
                    position: 0.068
                    color: "#37fff8"
                }

                GradientStop {
                    position: 1
                    color: "#c0fffd"
                }
            }
            MouseArea {
                anchors.fill: parent
            }
            border.width: 2

            Rectangle {
                id: chatTitleDivider
                x: 0
                width: 596
                height: 2
                color: "#ffffff"
                border.color: "#0c80f3"
                anchors.top: parent.top
                anchors.topMargin: 34
                anchors.right: parent.right
                anchors.rightMargin: 2
                border.width: 2
            }

            Text {
                id: chatTitle
                width: 320
                height: 37
                color: "#ffffff"
                text: qsTr("Chatroom")
                wrapMode: Text.WrapAnywhere
                font.family: "Verdana"
                verticalAlignment: Text.AlignVCenter
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 20
                textFormat: Text.AutoText
                font.pixelSize: 20
            }

            Rectangle {
                id: chatDisplayContainer
                x: 20
                y: 51
                width: 559
                height: 349
                color: "#ffffff"
                radius: 1
                TextArea {
                    id: chatDisplayText
                    tabChangesFocus: true
                    font.family: "Arial"
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: chatReplyContainer
                width: 442
                height: 42
                color: "#ffffff"
                radius: 1
                anchors.top: chatDisplayContainer.bottom
                anchors.topMargin: 26
                anchors.left: parent.left
                anchors.leftMargin: 20

                Flickable {
                    id: chatReplyScrollFix
                    width: 432
                    height: 32
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    contentWidth: chatReplyEdit.width
                    contentHeight: chatReplyEdit.height
                    clip: true

                    function ensureVisible(r)
                    {
                        if (contentX >= r.x)
                            contentX = r.x;
                        else if (contentX+width <= r.x+r.width)
                            contentX = r.x+r.width-width;
                        if (contentY >= r.y)
                            contentY = r.y;
                        else if (contentY+height <= r.y+r.height)
                            contentY = r.y+r.height-height;
                    }

                    TextEdit {
                        id: chatReplyEdit
                        width: chatReplyScrollFix.width
                        height: chatReplyScrollFix.height
                        text: qsTr("")
                        font.family: "Arial"
                        cursorVisible: true
                        wrapMode: TextEdit.Wrap
                        font.pixelSize: 12
                        onCursorRectangleChanged: chatReplyScrollFix.ensureVisible(cursorRectangle)
                    }
                }
            }

            Image {
                source: "images/window_controls/blue/button-blue.png"
                id: sendButton
                width: 90
                height: 42
                z: 3
                anchors.left: chatReplyContainer.right
                anchors.leftMargin: 17
                anchors.top: chatDisplayContainer.bottom
                anchors.topMargin: 26

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        sendButton.source = "images/window_controls/blue/button-blue-pressed.png"
                    }
                    onReleased: {
                        sendButton.source = "images/window_controls/blue/button-blue.png"
                        gApplicationController.sendButtonClicked(chatReplyEdit.text)
                    }
                }
            }
        }

        TabView {
            id: optionsTabs
            z: 2
            width: 200
            height: 510
            anchors.top: parent.top
            anchors.topMargin: 44
            anchors.left: parent.left
            anchors.leftMargin: 30
            /*MouseArea {
                anchors.fill: parent
            }*/
            Tab {
               id: userTab
               title: "Users"
               Rectangle {
                   radius: 10
                   color: "#00a9a2"
                   Rectangle {
                       id: usersContainer
                       anchors.top: parent.top
                       anchors.topMargin: 30
                       anchors.left: parent.left
                       anchors.leftMargin: 20
                       width: 160
                       height: 430
                       color: "#ffffff"
                       radius: 1
                   }
               }
           }
           Tab {
               id: configTab
               title: "Configuration"
               Rectangle {
                   radius: 10
                   color: "#00a9a2"

                   Text {
                       id: nickLabel
                       x: 42
                       width: 150
                       height: 20
                       color: "#ffffff"
                       text: qsTr("Nickname:")
                       wrapMode: Text.WrapAnywhere
                       font.family: "Verdana"
                       verticalAlignment: Text.AlignVCenter
                       anchors.top: parent.top
                       anchors.topMargin: 100
                       anchors.left: parent.left
                       anchors.leftMargin: 30
                       textFormat: Text.AutoText
                       font.pixelSize: 14
                   }

                   TextField {
                       id: nickText
                       width: 150
                       height: 20
                       anchors.top: nickLabel.top
                       anchors.topMargin: 30
                       anchors.left: parent.left
                       anchors.leftMargin: 30
                       style: TextFieldStyle {
                           textColor: "black"
                           background: Rectangle {
                               radius: 2
                               implicitWidth: nickText.width
                               implicitHeight: nickText.height
                               border.width: 0
                           }
                       }
                   }

                   Text {
                       id: avatarLabel
                       x: 42
                       width: 150
                       height: 20
                       color: "#ffffff"
                       text: qsTr("Avatar:")
                       wrapMode: Text.WrapAnywhere
                       font.family: "Verdana"
                       verticalAlignment: Text.AlignVCenter
                       anchors.top: nickText.top
                       anchors.topMargin: 50
                       anchors.left: parent.left
                       anchors.leftMargin: 30
                       textFormat: Text.AutoText
                       font.pixelSize: 14
                   }

                   SpinBox {
                       id: avatarNum
                       width: 40
                       height: 20
                       anchors.top: avatarLabel.top
                       anchors.topMargin: 30
                       anchors.left: parent.left
                       anchors.leftMargin: 30
                       minimumValue: 0
                       maximumValue: 21
                   }

                   Text {
                       id: ipLabel
                       x: 42
                       width: 160
                       height: 20
                       color: "#ffffff"
                       text: qsTr("Chatroom IP Address:")
                       wrapMode: Text.WrapAnywhere
                       font.family: "Verdana"
                       verticalAlignment: Text.AlignVCenter
                       anchors.top: avatarNum.top
                       anchors.topMargin: 50
                       anchors.left: parent.left
                       anchors.leftMargin: 30
                       textFormat: Text.AutoText
                       font.pixelSize: 14
                   }

                   TextField {
                       id: ipText
                       width: 150
                       height: 20
                       anchors.top: ipLabel.top
                       anchors.topMargin: 30
                       anchors.left: parent.left
                       anchors.leftMargin: 30
                       validator: RegExpValidator{regExp: /^(([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))\.){3}([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))$/}
                       style: TextFieldStyle {
                           textColor: "black"
                           background: Rectangle {
                               radius: 2
                               implicitWidth: nickText.width
                               implicitHeight: nickText.height
                               border.width: 0
                           }
                       }
                   }

                   Button {
                       id: buttonConnect
                       text: qsTr("Connect")
                       anchors.top: ipText.top
                       anchors.topMargin: 50
                       anchors.left: parent.left
                       anchors.leftMargin: 30
                       style: ButtonStyle {
                           background: Rectangle {
                               implicitWidth: 100
                               implicitHeight: 25
                               border.width: control.activeFocus ? 2 : 1
                               border.color: "#888"
                               radius: 4
                               gradient: Gradient {
                                   GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                                   GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                               }
                           }
                       }
                       onClicked : {
                            gApplicationController.connectButtonClicked(nickText.text, avatarNum.value, ipText.text)
                       }
                   }
               }
           }
           style: TabViewStyle {
               frameOverlap: 10
               tab: Rectangle {
                   color: styleData.selected ? "#00a9a2" : "#8ffdf9"
                   border.color:  "#00a9a2"
                   implicitWidth: tabText.width + 19
                   implicitHeight: 34
                   radius: 5
                   Text {
                       id: tabText
                       anchors.centerIn: parent
                       text: styleData.title
                       color: styleData.selected ? "white" : "black"
                       font.family: "Verdana"
                       font.pixelSize: 17
                   }
               }
               frame: Rectangle {
                   radius: 10
                   color: "#00a9a2"
               }
           }
        }
    }
}
