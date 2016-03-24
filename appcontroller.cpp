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
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <sstream>
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
    std::ostringstream convert;
    convert << icon;
    QString Result = QString::fromStdString(convert.str());
    gotNewMessage("Myself", username, Result, message);
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
    if (toFile) {
        std::time_t result = std::time(nullptr);
        std::string time = std::asctime(std::localtime(&result));
        time.pop_back();
        outfile << "[" << nickname.toStdString().c_str() << "/" << ip.toStdString().c_str() << " @ " << time << "] " << message.toStdString().c_str() << "\n";
    }
    //Someone said a new message.
    Message msg(ip, nickname, icon, message);
    addMessage(msg);
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
    //TODO: make it so when iterating through the list, qDebug() line is replaced with the "current online users" list.
    //Basically,
    //foreach user connected
    //    append it to the UI
    std::map<QString,QString>::iterator it;
    for (it=users.begin(); it!=users.end(); ++it)
        qDebug() << it->first << " => " << it->second;
}

void AppController::setMsg(QString text) {
    ushort code = 17;
    QChar *dc1 = new QChar(code);
    QRegExp rx(*dc1);
    QStringList components = text.split(rx);
    Message msg(components.at(0), components.at(1), components.at(2), components.at(3));
    addMessage(msg);
}

QString AppController::getMsg() const {
    return output;
}

void AppController::addMessage(const Message &msg)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  model << msg;
  endInsertRows();
}

int AppController::rowCount(const QModelIndex & parent) const {
  Q_UNUSED(parent);
  return model.count();
}

QVariant AppController::data(const QModelIndex & index, int role) const {
  if (index.row() < 0 || index.row() >= model.count())
      return QVariant();

  const Message &msg = model[index.row()];
  if (role == IpRole) {
      return msg.ip();
  } else if (role == UsrRole) {
      return msg.usr();
  } else if (role == AvRole) {
      return msg.av();
  } else if (role == TextRole) {
      return msg.tex();
  }
  return QVariant();
}

QHash<int, QByteArray> AppController::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[IpRole] = "ipAddress";
  roles[UsrRole] = "userName";
  roles[AvRole] = "imageSrc";
  roles[TextRole] = "textContents";
  return roles;
}

void AppController::test() {
    ushort code = 17;
    QChar *dc1 = new QChar(code);
    QString message;
    message += "127.0.12.123";
    message += *dc1;
    message += "testuser1";
    message += *dc1;
    message += QString::number(0);
    message += *dc1;
    message += "This is a wicked-cool awesome message!!";
    setMsg(message);
}
