#include <QDebug>
#include <QString>
#include <QObject>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include "appcontroller.h"

void * app;

QVariant AppController::getCursorPos()
{
    return QVariant(QCursor::pos());
}

void AppController::connectButtonClicked(const QString &usrname, int picNum, const QString &IP) {
    app = (void*)this;
    connectToServerWrapper(IP.toLatin1().data(),
                    (clientCodeCallback)newMessage,
                    (clientCodeCallback)recvNewUser,
                    (clientCodeCallback)recvUserLeft,
                    usrname.toLatin1().data(),
                    picNum);
    //Connect
}

void AppController::sendButtonClicked(const QString &message) {
    sendMessageWrapper(message.toLatin1().data());
    //Send
}

void AppController::gotNewMessage(const QString &ip, const QString &nickname, const QString &icon, const QString &message) {
    QString text = "[" + nickname + "] " + message + "\n";
    qDebug() << "[MESSAGE] IP: " << ip << ", Nickname: " << nickname << ", Icon: " << icon << ", Message: " << message;
    //Someone said a new message.
    //SetText here
    setText(text);
}

void AppController::gotNewUser(const QString &ip, const QString &nickname, const QString &icon) {
    qDebug() << "[NEWUSER] Nickname: " << nickname << ", Icon: " << icon << ", IP: " << ip;
    //New user connected
}

void AppController::gotLostUser(const QString &ip) {
    qDebug() << "[USERLEFT] IP: " << ip;
    //User disconnected
}

void AppController::setText(QString text) {

    ushort code = 17;
    QChar *dc1 = new QChar(code);
    QRegExp rx(*dc1);
    QStringList components = text.split(rx);

    model.append(components.at(0));
    model.append(components.at(1));
    model.append(QString::number(components.at(2)));
    model.append(components.at(3));

    textChanged(model);
}

QString AppController::getText() const {
    return output;
}
