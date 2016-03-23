#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//Packets have information sepearete by this delimiter (DC1)
#define MESSAGEDELIMITER ((char)17)
//Control flag for when a packet is about a new user joining (DC2)
#define NEWUSER ((char)18)
//Control flag for when a packet is about a user leaving (DC3)
#define USERLEFT ((char)19)
//Length of a message buffer
#define BUFLEN 255
//Length of our IP address string
#define IPLEN 17
//Length of a username
#define USERNAMELEN 26
//Length of the icon index, essentially 3 characters due to null character
#define ICONLEN 4
//Port number used when establishing a connection
#define PORTNO 7000

#endif //SHARED_H
