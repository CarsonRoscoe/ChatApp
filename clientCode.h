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
char rbuf[BUFLEN];
char username[USERNAMELEN];
int port, sd, icon;

//Thread variables
pthread_t thread;
int threadID;

//Callback declerations
typedef void (*clientCodeCallback)(char * message);
clientCodeCallback recvMessage;
clientCodeCallback newUser;
clientCodeCallback leftUser;

//Function definitions
void connectToServer(char * serverIP, int portNo, clientCodeCallback callback, clientCodeCallback newuserCallback, clientCodeCallback leftUser, char * username, int icon);
void sendMessage();
char * receiveMessage();
void closeConnection();
void * receiveThread(void * ptr);

#endif //CLIENTCODE_H
