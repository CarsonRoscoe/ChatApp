#include <QDebug>
#include <QString>
#include <QObject>
#include "appcontroller.h"

void * app;

QVariant AppController::getCursorPos()
{
    return QVariant(QCursor::pos());
}

void AppController::connectButtonClicked(const QString &usrname, int picNum, const QString &IP) {
    app = (void*)this;
    connectToServer(IP.toLatin1().data(),
                    (clientCodeCallback)newMessage,
                    (clientCodeCallback)recvNewUser,
                    (clientCodeCallback)recvUserLeft,
                    usrname.toLatin1().data(),
                    picNum);
    //Connect
}

void AppController::sendButtonClicked(const QString &message) {
    sendMessage(message.toLatin1().data());
    gotNewMessage("127.0.0.1", username, "" + icon, message);
    //Send
}

void AppController::gotNewMessage(const QString &ip, const QString &nickname, const QString &icon, const QString &message) {
    QString text = "[" + nickname + "] " + message + "<br>";
    qDebug() << "[MESSAGE] IP: " << ip << ", Nickname: " << nickname << ", Icon: " << icon << ", Message: " << message;
    //Someone said a new message.
    //SetText here
    setText(text);
}

void AppController::gotNewUser(const QString &ip, const QString &nickname, const QString &icon) {
    qDebug() << "[NEWUSER] Nickname: " << nickname << ", Icon: " << icon << ", IP: " << ip;
    users[ip] = nickname;
    updateUsers();
}

void AppController::gotLostUser(const QString &ip) {
    qDebug() << "[USERLEFT] IP: " << ip;
    users.erase(ip);
    updateUsers();
}

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
