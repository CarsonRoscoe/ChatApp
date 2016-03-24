#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QDebug>
#include "appcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    app.setWindowIcon(QIcon(":/images/misc/logonotext.ico"));
    AppController appcontext(&engine);

    ushort code = 17;
    QChar *dc1 = new QChar(code);
    QString message;
    message += "127.0.12.123";
    message += *dc1;
    message += "testuser1";
    message += *dc1;
    message += QString::number(5);
    message += *dc1;
    message += "This is a wicked-cool awesome message!!";
    appcontext.setText(message);

    return app.exec();
}
