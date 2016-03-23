#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlContext>
#include <QCursor>

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
};



#endif // APPCONTROLLER_H
