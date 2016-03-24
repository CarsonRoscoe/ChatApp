#include "message.h"

Message& Message::operator =(const Message& msg) {
    IP = msg.IP;
    usrName = msg.usrName;
    avatar = msg.avatar;
    message = msg.message;
    return *this;
}

QString Message::ip() const
{
  return IP;
}

QString Message::usr() const
{
  return usrName;
}

QString Message::av() const
{
  return avatar;
}

QString Message::tex() const
{
  return message;
}
