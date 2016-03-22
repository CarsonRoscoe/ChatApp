#ifndef CLIENTCODE_H
#define CLIENTCODE_H

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

//Defines
#define BUFLEN			255  	// Buffer length
#define USERNAMELEN 16

//Socket variables
char rbuf[BUFLEN];
char username[USERNAMELEN];
int port, sd;

//Thread variables
pthread_t thread;
int threadID;

//Callback declerations
typedef void (*clientCodeCallback)(char * message);
clientCodeCallback callback;

//Function definitions
void connectToServer(char * serverIP, int portNo, clientCodeCallback callback, char * username);
void sendMessage();
char * receiveMessage();
void closeConnection();
void * receiveThread(void * ptr);

#endif //CLIENTCODE_H
