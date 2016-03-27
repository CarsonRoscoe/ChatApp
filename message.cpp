/*------------------------------------------------------------------------------------------------------------------
-- Class: message.cpp
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
-- PROGRAMMER: Micah Willems / Carson Roscoe
--
-- METHODS: Message& Message::operator =(const Message& msg)
--          QString Message::ip() const
--          QString Message::usr() const
--          QString Message::av() const
--          QString Message::tex() const
--
-- NOTES:
-- The message class represents the information contained in a single element in a view, which includes ip address,
-- username, avatar number and text message. It has an assignment operator and four methods to access variables.
----------------------------------------------------------------------------------------------------------------------*/
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
