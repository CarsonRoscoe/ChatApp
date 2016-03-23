#ifndef SERVER_H
#define SERVER_H

#include "shared.h"
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

#define MAXCLIENTS	5

const char* CLEARSCREENANSI = "\e[1;1H\e[2J";
char addresses[MAXCLIENTS][IPLEN];
char usernames[MAXCLIENTS][USERNAMELEN];

// Function Prototypes
void CriticalError(char * errorMessage);
int main (int argc, char **argv);
void Refresh();
void InitializeAddresses();
void ClearUser(size_t i);

#endif //SERVER_H
