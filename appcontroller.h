#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlContext>
#include <QCursor>
#include <QDebug>
#include "clientqtwrappers.h"


class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList model READ getText WRITE setText NOTIFY textChanged)

    QQmlApplicationEngine *m_engine;

public:
    AppController(QQmlApplicationEngine *engine)
    {
        qDebug() << "initialized";
        engine->rootContext()->setContextProperty("gApplicationController", this);
        m_engine = engine;

    }
    Q_INVOKABLE QVariant getCursorPos();

public slots:
    void connectButtonClicked(const QString &usrname, int picNum, const QString &IP);
    void sendButtonClicked(const QString &message);
    void gotNewMessage(const QString &username, const QString &ip, const QString &icon, const QString &message);
    void gotNewUser(const QString &username, const QString &ip, const QString &icon);
    void gotLostUser(const QString &ip);
    void setText(QString text);
    QString getText() const;
signals:
    void textChanged(QString);
private:
    QStringList model;
    int numMsg;
};



#endif // APPCONTROLLER_H
