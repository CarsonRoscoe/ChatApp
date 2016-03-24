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

//Max number of clients connected
#define MAXCLIENTS	5
//String that, when printed, clears all ANSI terminals
const char* CLEARSCREENANSI = "\e[1;1H\e[2J";
//Array of IP addresses & usernames of the clients connected
char addresses[MAXCLIENTS][IPLEN];
char usernames[MAXCLIENTS][USERNAMELEN];

// Function Prototypes
int main (int argc, char **argv);
void CriticalError(char * errorMessage);
void Refresh();
void InitializeAddresses();
void ClearUser(size_t i);

#endif //SERVER_H
