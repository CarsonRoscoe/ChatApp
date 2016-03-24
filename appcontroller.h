#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlContext>
#include <QCursor>

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString newText READ getText WRITE setText NOTIFY textChanged)

public:
    AppController(QQmlApplicationEngine *engine)
    {
        engine->rootContext()->setContextProperty("gApplicationController", this);
    }
    Q_INVOKABLE QVariant getCursorPos();

public slots:
    void connectButtonClicked(const QString &usrname, int picNum, const QString &IP);
    void sendButtonClicked(const QString &message);
    void setText(QString text);
    QString getText() const;
signals:
    void textChanged(QString);
private:
    QString output;
};



#endif // APPCONTROLLER_H
