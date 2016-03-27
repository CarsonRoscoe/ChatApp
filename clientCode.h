#ifndef CLIENTCODE_H
#define CLIENTCODE_H

#include "shared.h"
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

//Socket variables
extern char rbuf[BUFLEN];
extern char username[USERNAMELEN];
extern int port, sd, icon;

//Thread variables
extern pthread_t thread;
extern int threadID;

//Callback declerations
typedef void (*clientCodeCallback)(char * message);
extern clientCodeCallback recvMessage;
extern clientCodeCallback newUser;
extern clientCodeCallback leftUser;

//Function definitions
void connectToServer(char * serverIP, clientCodeCallback recvCallback, clientCodeCallback newClientCallback, clientCodeCallback leftClientCallback, char * user, int ico) ;
void sendMessage(char * message);
char * receiveMessage();
void closeConnection();
void * receiveThread(void * ptr);

#endif //CLIENTCODE_H
