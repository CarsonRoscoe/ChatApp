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

/* Format for reply in "chatDisplayText"

Other people:

<div>
<div>
<image src="http://icons.iconarchive.com/icons/mattahan/ultrabuuf/128/Street-Fighter-Sakura-Kasugano-icon.png" style="float: left; margin-left: 10px; margin-top: -10px;" width=70 height=70/>
</div>
<div style="background-color: Turquoise; width: 410px; margin-left: 100px; margin-top: 20px; margin-bottom: 20px; border-radius: 10px; padding: 1px 15px;">
<p>
Test message content
</p>
</div>

You:

<div>
<image src="http://icons.iconarchive.com/icons/mattahan/ultrabuuf/128/Spaceship-Cylon-icon.png" style="float: right; margin-right: 10px; margin-top: -10px;" width=70 height=70/>
</div>
<div style="background-color: PaleTurquoise; width: 410px; margin-left: 20px; margin-top: 20px; margin-bottom: 20px; border-radius: 10px; padding: 1px 15px;">
<p>
Test message reply
</p>
</div>
</div>

QString *output = new QString("<div><div><image src=\"0.png\" style=\"float: left; margin-left: 10px;"
                             " margin-top: -10px;\" width=70 height=70/></div><div style=\"background-color:"
                             " Turquoise; width: 410px; margin-left: 100px; margin-top: 20px; margin-bottom:"
                             " 20px; border-radius: 10px; padding: 1px 15px;\"><p>Test message content</p>"
                             "</div>");

*/
