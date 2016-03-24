#ifndef CLIENTQTWRAPPERS_H
#define CLIENTQTWRAPPERS_H

#include "clientCode.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QMetaObject>

//Callback for clientCode to invoke on new message read
void newMessage(char * newmessage);
//Callback for clientCode to invoke on user left
void recvUserLeft(char * newmessage);
//Callback for clientCode to invoke on new user
void recvNewUser(char * newmessage);
void connectToServerWrapper(char * serverIP, clientCodeCallback recvCallback, clientCodeCallback newClientCallback, clientCodeCallback leftClientCallback, char * user, int ico);
void sendMessageWrapper(char * message);
void disconnectButtonClicked();

extern void * app;

#endif // CLIENTQTWRAPPERS_H
