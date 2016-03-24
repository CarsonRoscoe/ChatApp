/*------------------------------------------------------------------------------------------------------------------
-- Class: AppController
--
-- BaseClass: QObject
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created the initial class
--            March 23rd, 2016: Integrated class with networking code
--			  March 23rd, 2016: Commented
--
-- DESIGNER: Micah Willems
--
-- PROGRAMMER: Micah Willems
--
-- METHODS: AppController(QQmlApplicationEngine *engine)
--          Q_INVOKABLE QVariant getCursorPos()
--          void connectButtonClicked(const QString &usrname, int picNum, const QString &IP);
--          void sendButtonClicked(const QString &message);
--          void gotNewMessage(const QString &username, const QString &ip, const QString &icon, const QString &message);
--          void gotNewUser(const QString &username, const QString &ip, const QString &icon);
--          void gotLostUser(const QString &ip);
--          void setText(QString text);
--          QString getText() const;
--          void updateUsers();
--
-- NOTES:
-- AppController is what holds all of the functionality and logic of the GUI. It is the communications layer between
-- the business logic of the code and the visual UI side of the code.
----------------------------------------------------------------------------------------------------------------------*/

#include <QDebug>
#include <QString>
#include <QObject>
#include "appcontroller.h"

//Implementation of a extern used to hold the pointer to our window
void * app;

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: getCursorPos
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created the functionality
--			  March 23rd, 2016: Commented
--
-- DESIGNER: Micah Willems
--
-- PROGRAMMER: Micah Willems
--
-- INTERFACE: QVariant AppController::getCursorPos()
--
-- RETURN: QVariant which holds the pointer to the current mouse position
--
-- NOTES:
-- Gets the current x and y position of the mouse while over the window. Returns it as a QVariant.
----------------------------------------------------------------------------------------------------------------------*/
QVariant AppController::getCursorPos()
{
    return QVariant(QCursor::pos());
}

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: connectButtonClicked
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: [Micah] Created the method and hooked it up to the button
--			  March 23rd, 2016: [Carson] Integrated with the client networking code
--
-- DESIGNER: Micah Willems
--
-- PROGRAMMER: Micah Willems / Carson Roscoe
--
-- INTERFACE: void AppController::connectButtonClicked(nickname of the client
--                                                     icon index number
--                                                     IP address)
--
-- RETURN: void
--
-- NOTES:
-- Called when the connect button is clicked. Grabs the data from the form fields and passes it over to the function
-- used to connect to the server.
----------------------------------------------------------------------------------------------------------------------*/
void AppController::connectButtonClicked(const QString &usrname, int picNum, const QString &IP) {
    if (usrname == "" || picNum < 0 || IP == "")
        return;
    app = (void*)this;
    connectToServer(IP.toLatin1().data(),
                    (clientCodeCallback)newMessage,
                    (clientCodeCallback)recvNewUser,
                    (clientCodeCallback)recvUserLeft,
                    usrname.toLatin1().data(),
                    picNum);
}

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: sendButtonClicked
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: [Micah] Created the method and hooked it up to the button
--			  March 23rd, 2016: [Carson] Integrated with the client networking code
--
-- DESIGNER: Micah Willems
--
-- PROGRAMMER: Micah Willems / Carson Roscoe
--
-- INTERFACE: void AppController::sendButtonClicked(message to send)
--
-- RETURN: void
--
-- NOTES:
-- When the send button is clicked, send the message over to the networking code to send and pass our data to our
-- receive function to emulate receiving our packet (and therefor update the UI of our data)
----------------------------------------------------------------------------------------------------------------------*/
void AppController::sendButtonClicked(const QString &message) {
    if (message == "")
        return;
    sendMessage(message.toLatin1().data());
    gotNewMessage("0", username, "" + icon, message);
}

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: gotNewMessage
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created the method
--			  March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void AppController::gotNewMessage(IP message was received from
--                                              nickname of the sender
--                                              icon the sender was using as an index
--                                              message sent)
--
-- RETURN: void
--
-- NOTES:
-- After our read thread received a valid packet it deemed a packet of type message, it invoked this method call
-- on the main thread so the UI can be updated. This will update the UI with the new message.
----------------------------------------------------------------------------------------------------------------------*/
void AppController::gotNewMessage(const QString &ip, const QString &nickname, const QString &icon, const QString &message) {
    QString text = "[" + nickname + "] " + message + "<br>";
    qDebug() << "[MESSAGE] IP: " << ip << ", Nickname: " << nickname << ", Icon: " << icon << ", Message: " << message;
    //Someone said a new message.
    //SetText here
    setText(text);
}

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: gotNewUser
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created the method
--			  March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void AppController::gotNewUser(IP of the new user connected
--                                           nickname of the new user
--                                           icon the user was using as an index)
--
-- RETURN: void
--
-- NOTES:
-- After our read thread received a valid packet it deemed a packet of type new user, it invoked this method call
-- on the main thread so the UI can be updated. This will add the new user to our list and update the UI accordingly
----------------------------------------------------------------------------------------------------------------------*/
void AppController::gotNewUser(const QString &ip, const QString &nickname, const QString &icon) {
    qDebug() << "[NEWUSER] Nickname: " << nickname << ", Icon: " << icon << ", IP: " << ip;
    users[ip] = nickname;
    updateUsers();
}

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: gotLostUser
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created the method
--			  March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void AppController::gotLostUser(IP of the new user disconnected)
--
-- RETURN: void
--
-- NOTES:
-- After our read thread received a valid packet it deemed a packet of type user disconnected, it invoked this method call
-- on the main thread so the UI can be updated. This will remove the user from our list and update the UI accordingly
----------------------------------------------------------------------------------------------------------------------*/
void AppController::gotLostUser(const QString &ip) {
    qDebug() << "[USERLEFT] IP: " << ip;
    users.erase(ip);
    updateUsers();
}

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: updateUsers
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: [Carson] Created the functionality but printed updated list to console
--            March 23rd, 2016: [Micah] Hooked up the updated list to UI
--			  March 23rd, 2016: [Carson]Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe / Micah Willems
--
-- INTERFACE: void AppController::gotLostUser()
--
-- RETURN: void
--
-- NOTES:
-- Updates the UI of users after a user has been added or removed from our list of connected clients
----------------------------------------------------------------------------------------------------------------------*/
void AppController::updateUsers() {
    std::map<QString,QString>::iterator it;
    for (it=users.begin(); it!=users.end(); ++it)
        qDebug() << it->first << " => " << it->second;
}

void AppController::setText(QString text) {
    output += text;
    emit textChanged(output);
}

QString AppController::getText() const {
    return output;
}
