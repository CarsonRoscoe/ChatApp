#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlContext>
#include <QCursor>
#include <QStringList>
#include <QAbstractListModel>
#include <map>
#include <fstream>
#include "clientqtwrappers.h"
#include "message.h"

/*------------------------------------------------------------------------------------------------------------------
-- Class: AppController
--
-- BaseClass: QObject
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created the initial class
--            March 23rd, 2016: Integrated class with networking code
--			  March 23rd, 2016: Commented
--
-- DESIGNER: Micah Willems
--
-- PROGRAMMER: Micah Willems
--
-- METHODS: AppController(QQmlApplicationEngine *engine)
--          Q_INVOKABLE QVariant getCursorPos()
--          void connectButtonClicked(const QString &usrname, int picNum, const QString &IP);
--          void sendButtonClicked(const QString &message);
--          void gotNewMessage(const QString &username, const QString &ip, const QString &icon, const QString &message);
--          void gotNewUser(const QString &username, const QString &ip, const QString &icon);
--          void gotLostUser(const QString &ip);
--          void setText(QString text);
--          QString getText() const;
--          void updateUsers();
--
-- NOTES:
-- AppController is what holds all of the functionality and logic of the GUI. It is the communications layer between
-- the business logic of the code and the visual UI side of the code.
----------------------------------------------------------------------------------------------------------------------*/
class AppController : public QAbstractListModel
{
    Q_OBJECT

public:
    //Constructor which takes in the ApplicationEngine and assigns AppController to be the application controller of the engine
    AppController(QQmlApplicationEngine *engine, const char * fileName = NULL)
    {
        engine->rootContext()->setContextProperty("gApplicationController", this);
        if (fileName != NULL) {
            outfile.open(fileName, std::ofstream::out | std::ofstream::app);
            toFile = true;
        } else {
            toFile = false;
        }
    }

    void addMessage(const Message &msg);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;


    enum Roles {
              IpRole = Qt::UserRole + 1,
              UsrRole,
              AvRole,
              TextRole
          };

    QList<Message> model;

    //Returns the current cursor positions
    Q_INVOKABLE QVariant getCursorPos();
protected:
     QHash<int, QByteArray> roleNames() const;
public slots:
    //Connects to the server
    void connectButtonClicked(const QString &usrname, int picNum, const QString &IP);
    //Sends a message to the server
    void sendButtonClicked(const QString &message);
    //Callback invoked when a new message is received from server
    void gotNewMessage(const QString &username, const QString &ip, const QString &icon, const QString &message);
    //Callback invoked when a new user is connected
    void gotNewUser(const QString &username, const QString &ip, const QString &icon);
    //Callback invoked when a user is disconnected
    void gotLostUser(const QString &ip);
    //Method that updates the text area of the screen
    void setMsg(QString text);
    //Method that returns the text area of the screen
    QString getMsg() const;
    //Method that updates the list of all users in the UI
    void updateUsers();
    void test();
private:
    //String of the output
    QString output;
    //Map of all clients connected where the key is their IP address and the value is their nickname
    std::map<QString, QString> users;
    //File stream
    std::ofstream outfile;
    //Read to file or not
    bool toFile;
};

#endif // APPCONTROLLER_H
