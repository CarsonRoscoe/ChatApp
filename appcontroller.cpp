#include <QDebug>
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
