/*------------------------------------------------------------------------------------------------------------------
-- PROGRAM: ChatApp
--
-- FILE: main.cpp
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--            March 23rd, 2016: Commented
--
-- APPLICATION DESIGNER: Micah Willems / Carson Roscoe
--
-- APPLICATION PROGRAMMER: Micah Willems / Carson Roscoe
--
-- FUNCTIONS: int main(int argc, char *argv[])
--
-- NOTES:
-- ChatApp is a c++ chat application with a c server counterpart. The application takes advantage of the QT framework
-- for robust UI development. The networking side takes advantage of Linux sockets & the TCP protocol from the TCP/IP
-- protocol suite for transmitting data between the client and server.
--
-- The model used for the networking & UI pair was one that relies on callback functions for updating the UI. When a
-- connection is established, a posix thread is created that becomes our reading thread, and three function pointers
-- are passed in. Whenever the socket reads a message, it determines the type of message and invokes the appropriate
-- callback. This was done to have a clear seperation of the QT UI logic and the networking code for future reusability
-- of the networking code.
--
-- The bulk of the UI development was done in the main.qml file found in the Resources/qml.qrc// folder. The entire
-- UI was written in raw QML instead of using forms with a designer. The reason was QML gives you much more power
-- over the UI you want to design, however it also is much harder and a step lower than the designer/QSS alternative.
-- QML allows for scripting & function calls to be done inside the markup language, so a lot of our logic is held their.
----------------------------------------------------------------------------------------------------------------------*/
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QDebug>
#include "appcontroller.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--            March 23rd, 2016: Commented
--
-- DESIGNER: Micah Willems
--
-- PROGRAMMER: Micah Willems
--
-- FUNCTIONS: int main(int argc, char *argv[])
--
-- NOTES:
-- Starting point of our QT ChatApp application. The engine loads from a main.qml file, which contains all of the
-- visual markup of the application. After establishing the engine, AppController is created, which is our bridge
-- between the UI and the business logic. The engine is then passed into AppController, allowing it to have control
-- over the application.
----------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AppController appcontext(&engine, argc == 2 ? argv[1] : NULL);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    app.setWindowIcon(QIcon(":/images/misc/logonotext.ico"));

    return app.exec();
}
