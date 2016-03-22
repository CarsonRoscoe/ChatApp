#include "server.h"

int main (int argc, char **argv) {
	char * fail = "fail";
	int i, j, currentNewestClient, numReadibleDescriptors, bytesToRead, socketOptionSetting = 1;
	int listeningSocketDescriptor, newSocketDescriptor, curClientSocket, clientLength, port, clientLatestSocket, client[FD_SETSIZE];
	struct sockaddr_in server, clientAddress;
	char *bufPointer, buf[BUFLEN];
  ssize_t bytesRead;
  fd_set curSet, allSet;

	switch(argc) {
		case 1:
			port = SERVER_TCP_PORT;	// Use the default port
		break;
		case 2:
			port = atoi(argv[1]);	// Get user specified port
		break;
		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);
			exit(1);
	}

	// Create a stream socket
	if ((listeningSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		SystemFatal("Cannot Create Socket!");

  if (setsockopt (listeningSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &socketOptionSetting, sizeof(socketOptionSetting)) == -1)
          SystemFatal("setsockopt");

	// Bind an address to the socket
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

	if (bind(listeningSocketDescriptor, (struct sockaddr *)&server, sizeof(server)) == -1)
		SystemFatal("bind error");

	// Listen for connections
	// queue up to LISTENQ connect requests
	listen(listeningSocketDescriptor, LISTENQ);

	clientLatestSocket	= listeningSocketDescriptor;	// initialize
  currentNewestClient	= -1;		// index into client[] array

	for (i = 0; i < FD_SETSIZE; i++)
           	client[i] = -1;             // -1 indicates available entry

 	FD_ZERO(&allSet);
  FD_SET(listeningSocketDescriptor, &allSet);

	InitializeAddresses();

	while (TRUE) {
   	curSet = allSet;               // structure assignment
		numReadibleDescriptors = select(clientLatestSocket + 1, &curSet, NULL, NULL, NULL);

		if (FD_ISSET(listeningSocketDescriptor, &curSet)) { //New client connection
			clientLength = sizeof(clientAddress);
			if ((newSocketDescriptor = accept(listeningSocketDescriptor, (struct sockaddr *) &clientAddress, &clientLength)) == -1) {
				SystemFatal("accept error");
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

			Refresh();

			FD_SET (newSocketDescriptor, &allSet);     // add new descriptor to set
			if (newSocketDescriptor > clientLatestSocket) {
				clientLatestSocket = newSocketDescriptor;	// for select
			}

			if (i > currentNewestClient) {
				currentNewestClient = i;	// new max index in client[] array
			}

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
					printf("Remote Address:  %s closed connection\n", inet_ntoa(clientAddress.sin_addr));
					close(curClientSocket);
					FD_CLR(curClientSocket, &allSet);
					strcpy(addresses[i], "");
					Refresh();
					client[i] = -1;
					continue;
				}

				if (strcmp(buf, fail) == 0) {
					printf("Remote Address:  %s closed connection\n", inet_ntoa(clientAddress.sin_addr));
					close(curClientSocket);
					FD_CLR(curClientSocket, &allSet);
					strcpy(addresses[i], "");
					Refresh();
					client[i] = -1;
				} else {
					AddAddress(buf, inet_ntoa(clientAddress.sin_addr));
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

void AddAddress(char * message, char * address) {
	size_t i, j;
	char temp[BUFLEN];
	char newAddress[BUFLEN];

	sprintf(newAddress, "~%s", address);

	//Find where the message starts
	for(i = 0; message[i] != '\0'; i++)
		if (message[i] == ']')
			break;
		else
			temp[i] = message[i];

	for(j = 0; newAddress[j] != '\0'; j++)
		temp[i+j] = newAddress[j];

	for(; message[i] != '\0'; i++)
		temp[i+j] = message[i];

	temp[i+j] = '\0';

	//Put temp back into original message
	strncpy(message, temp, i+j);
}

void InitializeAddresses() {
	size_t i;
	for(i = 0; i < LISTENQ; i++)
		strcpy(addresses[i], "");
}

void Refresh() {
	size_t i;
	printf("%s", CLEARSCREENANSI);
	for(i = 0; i < LISTENQ; i++)
		if (addresses[i][0] != '\0')
			printf("%s\n", addresses[i]);
}

// Prints the error stored in errno and aborts the program.
void SystemFatal(const char* errorMessage) {
    perror (errorMessage);
    exit (EXIT_FAILURE);
}
