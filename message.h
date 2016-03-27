#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{

public:
    Message(QString ip, QString username, QString av, QString msg)
        : IP(ip), usrName(username), message(msg) {
        avatar = "images/icons/" + av + ".png";
    }
    Message& operator =(const Message& msg);
    QString ip() const;
    QString usr() const;
    QString av() const;
    QString tex() const;
private:
    QString IP;
    QString usrName;
    QString avatar;
    QString message;
};

#endif // MESSAGE_H
