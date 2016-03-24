#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlContext>
#include <QCursor>
#include "clientqtwrappers.h"


class AppController : public QObject
{
    Q_OBJECT

    //QQmlApplicationEngine m_engine;
public:
    AppController(QQmlApplicationEngine *engine)
    {
        engine->rootContext()->setContextProperty("gApplicationController", this);
    }
    Q_INVOKABLE QVariant getCursorPos();

public slots:
    void connectButtonClicked(const QString &usrname, int picNum, const QString &IP);
    void sendButtonClicked(const QString &message);
    void gotNewMessage(const QString &username, const QString &ip, const QString &icon, const QString &message);
    void gotNewUser(const QString &username, const QString &ip, const QString &icon);
    void gotLostUser(const QString &ip);
};



#endif // APPCONTROLLER_H
