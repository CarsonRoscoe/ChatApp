#include <QDebug>
#include <QString>
#include <QObject>
#include "appcontroller.h"

QVariant AppController::getCursorPos()
{
    return QVariant(QCursor::pos());
}

void AppController::connectButtonClicked(const QString &usrname, int picNum, const QString &IP) {
    qDebug() << usrname;
    qDebug() << picNum;
    qDebug() << IP;
    //Connect
}

void AppController::sendButtonClicked(const QString &message) {
    qDebug() << message;
    //Send
}

void AppController::setText(QString text) {
    output += text;
    emit textChanged(output);
}

QString AppController::getText() const {
    return output;
}
