#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include "appcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    app.setWindowIcon(QIcon(":/images/misc/logonotext.ico"));

    AppController appcontext(&engine);

    return app.exec();
}
