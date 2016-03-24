/*------------------------------------------------------------------------------------------------------------------
-- PROGRAM: server
--
-- FILE: server.c
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--						March 22nd, 2016: Added functionality regarding people leaving and joining
--						March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- FUNCTIONS: int main(int argc, char **argv)
--					  void InitializeAddresses()
--						void Refresh()
--						void ClearUser(size_t index)
--
--
-- NOTES:
-- This application is the server application used for ChatApp. The server communicates with clients via the
-- TCP protocol from the TCP/IP protocol suite. The server utilizes select for asynhronous communication with
-- multiple clients at a time.
--
-- The server acts as an echo server. It will echo all messages received from a client to all others, excluding the
-- one who sent it. When a client connects it will broadcast that the client connected to all other clients, as well
-- as when a client disconnects. When a client connects, it will receive messages telling it exactly how many people
-- are currently connected to the program and who they are.
----------------------------------------------------------------------------------------------------------------------*/
#include "server.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--						March 22nd, 2016: Added more functionality
--						March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: int main(amount of arguments passed in
--										 pointer to the arguments passed in)
--
-- RETURN: integer value in regards to a exit code
--
-- NOTES:
-- Starting point of our server application. It will first open a TCP socket connection to read from and bind it.
-- After preparing to handle the clients, it will utilize the select call to handle clients that are connecting,
-- disconnecting and wishing to echo a message to other clients.
----------------------------------------------------------------------------------------------------------------------*/
int main (int argc, char **argv) {
	//Prepare all variables that will be needed
	int currentNewestClient, numReadibleDescriptors, bytesToRead, socketOptionSetting = 1;
	int listeningSocketDescriptor, newSocketDescriptor, curClientSocket, port, clientLatestSocket, client[FD_SETSIZE];
	//Structures to hold server address and current client address
	struct sockaddr_in server, clientAddress;
	//Our buffer that will hold data and a pointer that will point to our buffer
	char *bufPointer, buf[BUFLEN];
	//Bytes read. ssize_t allows our unsigned value to also hold -1 if needed for error checking
  ssize_t bytesRead;
	//Holds the different checks needed for select
  fd_set curSet, allSet;
	//Temporary variables used when iterating through for loops
	int i, j;
	//Client socket length
	socklen_t clientLength;

	//Check if the user specified a port or not. If not, use the default.
	switch(argc) {
		//No port specified, use the default
		case 1:
			port = PORTNO;
		break;
		//A port specified, use that port
		case 2:
			port = atoi(argv[1]);
		break;
		//More than one argument, something is wrong. Explain usage and exit
		default:
			fprintf(stderr, "Usage: %s [(optional)port]\n", argv[0]);
			exit(1);
	}

	//Create a stream socket
	if ((listeningSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		CriticalError("Error creating the socket");

	//Set the socket options ont he descriptor as needed
  if (setsockopt (listeningSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &socketOptionSetting, sizeof(socketOptionSetting)) == -1)
    CriticalError("Error calling setsockopt()");

	//Prepare to bind the socket
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	//Accept all connections from all clients
	server.sin_addr.s_addr = htonl(INADDR_ANY);
  //Bind our address to the socket
	if (bind(listeningSocketDescriptor, (struct sockaddr *)&server, sizeof(server)) == -1)
		CriticalError("bind error");

	//Listen for connections. Allow up to MAXCLIENTS connections
	listen(listeningSocketDescriptor, MAXCLIENTS);

	//Set our latest client to the listening we just created, as our first will be on this
	clientLatestSocket	= listeningSocketDescriptor;
	//Set our latest clients index to -1, as we have no clients yet
  currentNewestClient	= -1;

	//Default all clients in the array to -1, as they are all unused at the start and available
	for (i = 0; i < FD_SETSIZE; i++)
           	client[i] = -1;

  //Prepare for checking FD_ISSET
 	FD_ZERO(&allSet);
  FD_SET(listeningSocketDescriptor, &allSet);

	//Set all our storage of addresses and usernames to be empty
	InitializeAddresses();

	printf("Starting Server\n");

	//Begin our forever loop listening for connections & data
	while (1) {
   	curSet = allSet;
		numReadibleDescriptors = select(clientLatestSocket + 1, &curSet, NULL, NULL, NULL);

		if (FD_ISSET(listeningSocketDescriptor, &curSet)) { //New client connection
			printf("New client\n");
			clientLength = sizeof(clientAddress);
			if ((newSocketDescriptor = accept(listeningSocketDescriptor, (struct sockaddr *) &clientAddress, &clientLength)) == -1) {
				CriticalError("accept error");
			}

			for(j = 0; j <= currentNewestClient; j++) {
				char newbuf[BUFLEN];
				sprintf(newbuf, "%c%s%c%s", NEWUSER, usernames[j], MESSAGEDELIMITER, addresses[j]);
				write(newSocketDescriptor, newbuf, BUFLEN);   // echo to client
			}



      for (i = 0; i < FD_SETSIZE; i++) {
				if (client[i] < 0) {
					client[i] = newSocketDescriptor;	// save descriptor
					break;
        }
			}

			if (i == FD_SETSIZE) {
				printf ("Too many clients\n");
        exit(1);
			} else {
				strcpy(addresses[i], inet_ntoa(clientAddress.sin_addr));
			}

			FD_SET (newSocketDescriptor, &allSet);     // add new descriptor to set
			if (newSocketDescriptor > clientLatestSocket)
				clientLatestSocket = newSocketDescriptor;	// for select

			if (i > currentNewestClient)
				currentNewestClient = i;	// new max index in client[] array

			Refresh();

			if (--numReadibleDescriptors <= 0)
				continue;	// no more readable descriptors
		}

		for (i = 0; i <= currentNewestClient; i++)	{ //Check all clients for data
			if ((curClientSocket = client[i]) < 0)
				continue;
			if (FD_ISSET(curClientSocket, &curSet)) {
   			bufPointer = buf;
				bytesToRead = BUFLEN;
				while ((bytesRead = read(curClientSocket, bufPointer, bytesToRead)) > 0) {
					bufPointer += bytesRead;
					bytesToRead -= bytesRead;
				}

				if (strlen(buf) == 0 && bytesRead == 0) {
					char newbuf[BUFLEN];

					sprintf(newbuf, "%c%s%c", USERLEFT, inet_ntoa(clientAddress.sin_addr));
					//printf("%s", newbuf);

					for(j = 0; j <= currentNewestClient; j++) {
						if (curClientSocket != client[j]) {
							write(client[j], newbuf, BUFLEN);   // echo to client
						}
					}

					close(curClientSocket);
					FD_CLR(curClientSocket, &allSet);
					ClearUser(i);
					Refresh();
					client[i] = -1;
					continue;
				}

				if (buf[0] == NEWUSER) {
					char newbuf[BUFLEN];
					char token = MESSAGEDELIMITER;
					sprintf(newbuf, "%s%c%s", buf, MESSAGEDELIMITER, inet_ntoa(clientAddress.sin_addr));
					for(j = 0; j <= currentNewestClient; j++) {
						if (curClientSocket != client[j]) {
							write(client[j], newbuf, BUFLEN);   // echo to client
						}
					}
					memmove(buf, buf + 1, strlen(buf));
					strcpy(usernames[i], strtok(buf, &token));
					Refresh();
				} else {
					char newbuf[BUFLEN];
					sprintf(newbuf, "%s%c%s", inet_ntoa(clientAddress.sin_addr), MESSAGEDELIMITER, buf);
					sprintf(buf, newbuf);
					for(j = 0; j <= currentNewestClient; j++) {
						if (curClientSocket != client[j]) {
							write(client[j], buf, BUFLEN);   // echo to client
						}
					}
				}

				buf[0] = '\0';

				if (--numReadibleDescriptors <= 0)
        	break;        // no more readable descriptors
			}
    }
  }
	return(0);
}

void InitializeAddresses() {
	size_t i;
	for(i = 0; i < MAXCLIENTS; i++)
		strcpy(addresses[i], "");
	printf("Initialized Addresses\n");
}

void Refresh() {
	size_t i;
	printf("%s", CLEARSCREENANSI);
	for(i = 0; i < MAXCLIENTS; i++)
		if (addresses[i][0] != '\0')
			printf("Address: %s - Nickname: %s\n", addresses[i], usernames[i]);
	printf("Refreshed Screen\n");
}

void ClearUser(size_t index) {
	strcpy(addresses[index], "");
	strcpy(usernames[index], "");
	printf("Clear Users\n");
}

// Prints the error stored in errno and aborts the program.
void CriticalError(char * errorMessage) {
    fprintf(stderr, errorMessage);
    exit(1);
}
