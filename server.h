#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define SERVER_TCP_PORT 7000	// Default port
#define BUFLEN	255		//Buffer length
#define IPLEN 17
#define TRUE	1
#define LISTENQ	5
#define MAXLINE 4096

const char* CLEARSCREENANSI = "\e[1;1H\e[2J";
char addresses[LISTENQ][IPLEN];

// Function Prototypes
void SystemFatal(const char * errorMessage );
int main (int argc, char **argv);
void AddAddress(char * message, char * address);
void Refresh();
void InitializeAddresses();

#endif //SERVER_H
